#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Include the generated header
#include "test.h"

typedef struct yaml_node {
    char* key;
    char* value;
    char* type;
    bool is_array_item;
    int array_index;
    struct yaml_node** children;
    int child_count;
    int child_capacity;
} yaml_node_t;

yaml_node_t* root_node = NULL;

yaml_node_t* create_node(const char* key, const char* value, const char* type) {
    yaml_node_t* node = malloc(sizeof(yaml_node_t));
    node->key = key ? strdup(key) : NULL;
    node->value = value ? strdup(value) : NULL;
    node->type = type ? strdup(type) : NULL;
    node->is_array_item = false;
    node->array_index = -1;
    node->children = NULL;
    node->child_count = 0;
    node->child_capacity = 0;
    return node;
}

void add_child(yaml_node_t* parent, yaml_node_t* child) {
    if (parent->child_count >= parent->child_capacity) {
        parent->child_capacity = parent->child_capacity == 0 ? 4 : parent->child_capacity * 2;
        parent->children = realloc(parent->children, parent->child_capacity * sizeof(yaml_node_t*));
    }
    parent->children[parent->child_count++] = child;
}

yaml_node_t* find_or_create_child(yaml_node_t* parent, const char* key) {
    for (int i = 0; i < parent->child_count; i++) {
        if (parent->children[i]->key && strcmp(parent->children[i]->key, key) == 0) {
            return parent->children[i];
        }
    }
    
    yaml_node_t* child = create_node(key, NULL, NULL);
    add_child(parent, child);
    return child;
}

void build_tree_from_fields() {
    const yaml_field_t* fields = get_yaml_fields();
    int count = get_yaml_field_count();
    
    root_node = create_node(NULL, NULL, NULL);
    
    for (int i = 0; i < count; i++) {
        const char* key_path = fields[i].key;
        const char* type = fields[i].type;
        const char* value = fields[i].value;
        
        if (!key_path || !value) continue;
        
        char* path_copy = strdup(key_path);
        yaml_node_t* current = root_node;
        
        char* saveptr;
        char* token = strtok_r(path_copy, ".", &saveptr);
        
        while (token != NULL) {
            char* next_token = strtok_r(NULL, ".", &saveptr);
            
            char* bracket = strchr(token, '[');
            if (bracket) {
                *bracket = '\0';
                char* index_str = bracket + 1;
                char* end_bracket = strchr(index_str, ']');
                if (end_bracket) *end_bracket = '\0';
                
                int index = atoi(index_str);
                
                yaml_node_t* array_node = find_or_create_child(current, token);
                if (!array_node->type) {
                    array_node->type = strdup("array");
                }

                char item_key[32];
                snprintf(item_key, sizeof(item_key), "[%d]", index);
                yaml_node_t* item_node = find_or_create_child(array_node, item_key);
                item_node->is_array_item = true;
                item_node->array_index = index;
                
                if (next_token == NULL) {
                    if (item_node->value) free(item_node->value);
                    item_node->value = strdup(value);
                    if (item_node->type) free(item_node->type);
                    item_node->type = strdup(type);
                }
                current = item_node;
            } else {
                current = find_or_create_child(current, token);
                if (next_token == NULL) {
                    if (current->value) free(current->value);
                    current->value = strdup(value);
                    if (current->type) free(current->type);
                    current->type = strdup(type);
                }
            }
            
            token = next_token;
        }
        
        free(path_copy);
    }
}

void print_yaml_node(FILE* fp, yaml_node_t* node, int indent, bool is_array_item) {
    if (!node) return;
    
    for (int i = 0; i < indent; i++) {
        fprintf(fp, "  ");
    }
    
    if (is_array_item) {
        fprintf(fp, "- ");
        if (node->child_count > 0) {
            fprintf(fp, "\n");
            // Print children with extra indentation
            for (int i = 0; i < node->child_count; i++) {
                yaml_node_t* child = node->children[i];
                print_yaml_node(fp, child, indent + 1, false);
            }
            return;
        }
    } else if (node->key && !strstr(node->key, "[")) {
        fprintf(fp, "%s:", node->key);
    }
    
    // If this node has a value, print it
    if (node->value && node->type) {
        if (strcmp(node->type, "string") == 0) {
            if (strchr(node->value, '\n')) {
                fprintf(fp, " |\n");
                const char* current = node->value;
                const char* line_start = current;
                while (*current != '\0') {
                    if (*current == '\n') {
                        for (int i = 0; i <= indent; i++) {
                            fprintf(fp, "  ");
                        }
                        
                        int line_length = current - line_start;
                        if (line_length > 0) {
                            fprintf(fp, "%.*s\n", line_length, line_start);
                        } else {
                            fprintf(fp, "\n");
                        }
                        
                        line_start = current + 1;
                    }
                    current++;
                }
                if (line_start < current) {
                    for (int i = 0; i <= indent; i++) {
                        fprintf(fp, "  ");
                    }
                    fprintf(fp, "%s\n", line_start);
                }
                
                return;
            } else {
                bool needs_quotes = (strchr(node->value, ':') != NULL || 
                                   strchr(node->value, '#') != NULL ||
                                   strcmp(node->value, "true") == 0 ||
                                   strcmp(node->value, "false") == 0 ||
                                   strcmp(node->value, "null") == 0);
                
                if (needs_quotes) {
                    fprintf(fp, " \"%s\"\n", node->value);
                } else {
                    fprintf(fp, " %s\n", node->value);
                }
            }
        } else {
            fprintf(fp, " %s\n", node->value);
        }
        return;
    }
    
    if (node->child_count > 0) {
        if (!is_array_item && node->key && !strstr(node->key, "[")) {
            fprintf(fp, "\n");
        }

        bool is_array = (node->type && strcmp(node->type, "array") == 0);
        
        for (int i = 0; i < node->child_count; i++) {
            yaml_node_t* child = node->children[i];
            bool child_is_array_item = is_array && child->is_array_item;
            print_yaml_node(fp, child, is_array_item ? indent : indent + 1, child_is_array_item);
        }
    } else if (!node->value && node->key && !strstr(node->key, "[")) {
        fprintf(fp, " null\n");
    }
}

void free_node(yaml_node_t* node) {
    if (!node) return;
    
    free(node->key);
    free(node->value);
    free(node->type);
    
    for (int i = 0; i < node->child_count; i++) {
        free_node(node->children[i]);
    }
    free(node->children);
    free(node);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <output_yaml_file>\n", argv[0]);
        return 1;
    }

    const char* output_filename = argv[1];
    FILE* fp = fopen(output_filename, "w");
    if (!fp) {
        perror("Error opening output file");
        return 1;
    }

    printf("Generating YAML file: %s\n", output_filename);

    build_tree_from_fields();
    
    if (!root_node || root_node->child_count == 0) {
        fprintf(fp, "# No YAML fields found\n");
        fclose(fp);
        printf("Warning: No YAML fields found in generated header\n");
        return 0;
    }
    
    fprintf(fp, "# Reconstructed YAML from C header\n");
    fprintf(fp, "# Full hierarchical reconstruction\n\n");
    for (int i = 0; i < root_node->child_count; i++) {
        print_yaml_node(fp, root_node->children[i], 0, false);
    }

    free_node(root_node);
    
    fclose(fp);
    printf("Successfully generated %s\n", output_filename);

    return 0;
}
