import yaml
import sys
import os
import re
from typing import Dict, Any, List, Union, Tuple

class GenericYAMLToCConverter:
    def __init__(self):
        self.yaml_data = None
        self.header_content = []
        self.struct_definitions = set()
        self.array_definitions = []
        self.generated_arrays = {}
        self.array_prefix = ""
    
    def load_yaml_file(self, yaml_path: str) -> bool:
        try:
            with open(yaml_path, 'r') as file:
                self.yaml_data = yaml.safe_load(file)
            return True
        except Exception as e:
            print(f"Error loading YAML file: {e}")
            return False
    
    def sanitize_identifier(self, name: str) -> str:
        if not name:
            return "UNKNOWN"
        sanitized = re.sub(r'[^a-zA-Z0-9_]', '_', str(name))
        if sanitized and sanitized[0].isdigit():
            sanitized = '_' + sanitized
        return sanitized
    
    def escape_c_string(self, text: Any) -> str:
        if text is None:
            return "NULL"
        
        text_str = str(text)
        if '\n' in text_str:
            lines = text_str.split('\n')
            escaped_lines = []
            for line in lines:
                escaped = line.replace('\\', '\\\\').replace('"', '\\"').replace('\t', '\\t').replace('\r', '\\r')
                escaped_lines.append(f'"{escaped}\\n"')
            
            if len(escaped_lines) == 0:
                return '""'
            elif len(escaped_lines) == 1:
                return escaped_lines[0]
            else:
                return ' \\\n    '.join(escaped_lines)
        else:
            escaped = text_str.replace('\\', '\\\\').replace('"', '\\"').replace('\n', '\\n').replace('\r', '\\r').replace('\t', '\\t')
            return f'"{escaped}"'
    
    def determine_c_type(self, value: Any) -> str:
        if isinstance(value, bool):
            return "bool"
        elif isinstance(value, int):
            return "int"
        elif isinstance(value, float):
            return "double"
        elif isinstance(value, str):
            return "const char*"
        elif isinstance(value, list):
            return "void*"
        elif isinstance(value, dict):
            return "void*"
        else:
            return "const char*"
    
    def get_c_value(self, value: Any) -> str:
        if value is None:
            return "NULL"
        elif isinstance(value, bool):
            return "true" if value else "false"
        elif isinstance(value, (int, float)):
            return str(value)
        elif isinstance(value, str):
            return self.escape_c_string(value)
        elif isinstance(value, (list, dict)):
            return "NULL"
        else:
            return self.escape_c_string(str(value))
    
    def generate_array_for_list(self, key: str, items: List[Any], prefix: str) -> str:
        if not items:
            return ""
        
        array_name = f"{prefix}_{self.sanitize_identifier(key)}"
        self.generated_arrays[key] = array_name
        
        if all(isinstance(item, (str, int, float, bool)) for item in items):
            item_type = self.determine_c_type(items[0]) if items else "const char*"
            self.array_definitions.append(f"static const {item_type} {array_name}[] = {{")
            
            for i, item in enumerate(items):
                comma = "," if i < len(items) - 1 else ""
                self.array_definitions.append(f"    {self.get_c_value(item)}{comma}")
            
            self.array_definitions.append("};")
            self.array_definitions.append(f"#define {array_name.upper()}_COUNT {len(items)}")
            self.array_definitions.append("")
            return array_name
        else:
            struct_name = f"{self.sanitize_identifier(key)}_item_t"
            if struct_name not in self.struct_definitions:
                self.generate_generic_struct_for_objects(key, items)
                self.struct_definitions.add(struct_name)
            
            self.array_definitions.append(f"static const {struct_name} {array_name}[] = {{")
            
            for i, item in enumerate(items):
                comma = "," if i < len(items) - 1 else ""
                self.array_definitions.append(f"    {self.generate_struct_initializer(item)}{comma}")
            
            self.array_definitions.append("};")
            self.array_definitions.append(f"#define {array_name.upper()}_COUNT {len(items)}")
            self.array_definitions.append("")
            return array_name
    
    def generate_generic_struct_for_objects(self, key: str, objects: List[Dict]) -> None:
        if not objects:
            return
        
        all_keys = set()
        for obj in objects:
            if isinstance(obj, dict):
                all_keys.update(obj.keys())
        
        struct_name = f"{self.sanitize_identifier(key)}_item_t"
        self.header_content.append(f"// Structure for {key} items")
        self.header_content.append(f"typedef struct {{")
        
        for obj_key in sorted(all_keys):
            field_name = self.sanitize_identifier(obj_key)
            sample_value = None
            for obj in objects:
                if isinstance(obj, dict) and obj_key in obj:
                    sample_value = obj[obj_key]
                    break
            
            c_type = self.determine_c_type(sample_value)
            self.header_content.append(f"    {c_type} {field_name};")
        
        self.header_content.append(f"}} {struct_name};")
        self.header_content.append("")
    
    def generate_struct_initializer(self, obj: Dict[str, Any]) -> str:
        if not isinstance(obj, dict):
            return "{}"
        
        initializers = []
        for key, value in obj.items():
            field_name = self.sanitize_identifier(key)
            c_value = self.get_c_value(value)
            initializers.append(f".{field_name} = {c_value}")
        
        return "{\n        " + ",\n        ".join(initializers) + "\n    }"
    
    def analyze_data_structure(self, data: Dict[str, Any], path: str = "") -> Dict[str, Any]:
        structure_info = {}
        
        for key, value in data.items():
            if key.startswith('$') or key.startswith('#'):
                continue
                
            current_path = f"{path}.{key}" if path else key
            field_name = self.sanitize_identifier(key)
            
            if isinstance(value, dict):
                struct_name = f"{self.sanitize_identifier(current_path.replace('.', '_'))}_t"
                structure_info[field_name] = {
                    'type': 'struct',
                    'struct_name': struct_name,
                    'nested': self.analyze_data_structure(value, current_path)
                }
            elif isinstance(value, list):
                structure_info[field_name] = {
                    'type': 'array',
                    'array_name': f"{field_name}_array",
                    'count_name': f"{field_name}_count",
                    'count': len(value)
                }
            else:
                structure_info[field_name] = {
                    'type': 'simple',
                    'c_type': self.determine_c_type(value),
                    'value': value
                }
        
        return structure_info
    
    def generate_nested_struct_definition(self, struct_name: str, structure_info: Dict[str, Any]) -> None:
        if struct_name in self.struct_definitions:
            return
        
        for field_name, field_info in structure_info.items():
            if field_info['type'] == 'struct':
                nested_struct_name = field_info['struct_name']
                self.generate_nested_struct_definition(nested_struct_name, field_info['nested'])
        
        self.header_content.append(f"// Structure definition for {struct_name}")
        self.header_content.append(f"typedef struct {{")
        
        for field_name, field_info in structure_info.items():
            if field_info['type'] == 'struct':
                nested_struct_name = field_info['struct_name']
                self.header_content.append(f"    {nested_struct_name} {field_name};")
            elif field_info['type'] == 'array':
                array_field = field_info.get('array_name', f"{field_name}_array")
                count_field = field_info.get('count_name', f"{field_name}_count")
                self.header_content.append(f"    const void* {array_field};")
                self.header_content.append(f"    int {count_field};")
            else:
                c_type = field_info['c_type']
                self.header_content.append(f"    {c_type} {field_name};")
        
        self.header_content.append(f"}} {struct_name};")
        self.header_content.append("")
        self.struct_definitions.add(struct_name)

    def generate_data_arrays(self, data: Dict[str, Any], prefix: str) -> None:
        self.array_prefix = prefix
        self._generate_arrays_recursive(data, prefix, "")
    
    def _generate_arrays_recursive(self, data: Dict[str, Any], prefix: str, path: str) -> None:
        for key, value in data.items():
            if key.startswith('$') or key.startswith('#'):
                continue
                
            current_path = f"{path}_{key}" if path else key
            
            if isinstance(value, list):
                self.generate_array_for_list(current_path, value, prefix)
            elif isinstance(value, dict):
                self._generate_arrays_recursive(value, prefix, current_path)
    
    def generate_main_structure(self, data: Dict[str, Any], struct_name: str) -> None:
        structure_info = self.analyze_data_structure(data)
        
        for field_name, field_info in structure_info.items():
            if field_info['type'] == 'struct':
                self.generate_nested_struct_definition(field_info['struct_name'], field_info['nested'])
        
        self.header_content.append(f"// Main YAML data structure")
        self.header_content.append(f"typedef struct {{")
        
        for field_name, field_info in structure_info.items():
            if field_info['type'] == 'struct':
                struct_type = field_info['struct_name']
                self.header_content.append(f"    {struct_type} {field_name};")
            elif field_info['type'] == 'array':
                self.header_content.append(f"    const void* {field_name}_array;")
                self.header_content.append(f"    int {field_name}_count;")
            else:
                c_type = field_info['c_type']
                self.header_content.append(f"    {c_type} {field_name};")
        
        self.header_content.append(f"}} {struct_name};")
        self.header_content.append("")
    
    def generate_main_instance(self, data: Dict[str, Any], instance_name: str, struct_name: str) -> None:
        self.header_content.append(f"// Main data instance")
        self.header_content.append(f"static const {struct_name} {instance_name} = {{")
        
        self.generate_instance_initialization(data, instance_name, "")
        
        self.header_content.append("};")
        self.header_content.append("")
    
    def generate_instance_initialization(self, data: Dict[str, Any], instance_name: str, path: str) -> None:
        for key, value in data.items():
            if key.startswith('$') or key.startswith('#'):
                continue
                
            field_name = self.sanitize_identifier(key)
            current_path = f"{path}_{key}" if path else key
            
            if isinstance(value, dict):
                self.header_content.append(f"    .{field_name} = {{")
                self.generate_nested_initialization(value, instance_name, current_path, "        ")
                self.header_content.append("    },")
            elif isinstance(value, list):
                array_name = f"{self.array_prefix}_{self.sanitize_identifier(current_path)}"
                self.header_content.append(f"    .{field_name}_array = {array_name},")
                self.header_content.append(f"    .{field_name}_count = {len(value)},")
            else:
                c_value = self.get_c_value(value)
                self.header_content.append(f"    .{field_name} = {c_value},")
    
    def generate_nested_initialization(self, data: Dict[str, Any], instance_name: str, path: str, indent: str) -> None:
        structure_info = self.analyze_data_structure(data)
        
        for key, value in data.items():
            if key.startswith('$') or key.startswith('#'):
                continue
                
            field_name = self.sanitize_identifier(key)
            current_path = f"{path}_{key}"
            
            if isinstance(value, dict):
                self.header_content.append(f"{indent}.{field_name} = {{")
                self.generate_nested_initialization(value, instance_name, current_path, indent + "    ")
                self.header_content.append(f"{indent}}},")
            elif isinstance(value, list):
                field_info = structure_info.get(field_name, {})
                array_field = field_info.get('array_name', f"{field_name}_array")
                count_field = field_info.get('count_name', f"{field_name}_count")
                
                array_name = f"{self.array_prefix}_{self.sanitize_identifier(current_path)}"
                
                self.header_content.append(f"{indent}.{array_field} = {array_name},")
                self.header_content.append(f"{indent}.{count_field} = {len(value)},")
            else:
                c_value = self.get_c_value(value)
                self.header_content.append(f"{indent}.{field_name} = {c_value},")
    
    def collect_yaml_fields_comprehensive(self, data: Any, fields: List[Tuple[str, str, Any]], path: str) -> None:
        if isinstance(data, dict):
            for key, value in data.items():
                if key.startswith('$') or key.startswith('#'):
                    continue
                
                current_path = f"{path}.{key}" if path else key
                
                if isinstance(value, (str, int, float, bool)):
                    yaml_type = "string" if isinstance(value, str) else type(value).__name__
                    fields.append((current_path, yaml_type, value))
                elif isinstance(value, list):
                    # Add each list item with proper indexing
                    for i, item in enumerate(value):
                        item_path = f"{current_path}[{i}]"
                        if isinstance(item, (str, int, float, bool)):
                            item_type = "string" if isinstance(item, str) else type(item).__name__
                            fields.append((item_path, item_type, item))
                        elif isinstance(item, dict):
                            # Recursively process nested dictionaries within arrays
                            self.collect_yaml_fields_comprehensive(item, fields, item_path)
                        else:
                            fields.append((item_path, "unknown", str(item)))
                elif isinstance(value, dict):
                    # Recursively process nested dictionaries
                    self.collect_yaml_fields_comprehensive(value, fields, current_path)
                else:
                    fields.append((current_path, "unknown", str(value)))

    
    def generate_yaml_reconstruction_data(self, data: Dict[str, Any]) -> None:
        """Generate comprehensive YAML reconstruction data"""
        self.header_content.append("// Comprehensive YAML field reconstruction")
        self.header_content.append("typedef struct {")
        self.header_content.append("    const char* key;")
        self.header_content.append("    const char* type;")
        self.header_content.append("    const char* value;")
        self.header_content.append("} yaml_field_t;")
        self.header_content.append("")
        
        fields = []
        self.collect_yaml_fields_comprehensive(data, fields, "")
        
        if fields:
            self.header_content.append("static const yaml_field_t yaml_fields[] = {")
            
            for i, (key, yaml_type, value) in enumerate(fields):
                comma = "," if i < len(fields) - 1 else ""
                key_str = self.escape_c_string(key)
                type_str = self.escape_c_string(yaml_type)
                value_str = self.escape_c_string(str(value)) if value is not None else "NULL"
                
                self.header_content.append(f"    {{{key_str}, {type_str}, {value_str}}}{comma}")
            
            self.header_content.append("};")
            self.header_content.append(f"#define YAML_FIELDS_COUNT {len(fields)}")
            self.header_content.append("")
    
    def convert_to_c_header(self) -> str:
        if not self.yaml_data:
            return ""
        
        data = self.yaml_data
        
        base_name = "unknown"
        if 'name' in data:
            base_name = str(data['name'])
        elif isinstance(data, dict) and len(data) > 0:
            for key in data.keys():
                if not key.startswith('$') and not key.startswith('#'):
                    base_name = str(key)
                    break
        
        sanitized_name = self.sanitize_identifier(base_name)

        self.header_content = []
        self.array_definitions = []
        self.struct_definitions = set()
        self.generated_arrays = {}
        
        self.generate_header_guard(sanitized_name)
        
        self.generate_data_arrays(data, sanitized_name.lower())

        self.generate_main_structure(data, f"{sanitized_name.lower()}_data_t")

        self.header_content.extend(self.array_definitions)
        
        self.generate_main_instance(data, f"{sanitized_name.lower()}_data", f"{sanitized_name.lower()}_data_t")

        self.generate_yaml_reconstruction_data(data)

        self.generate_access_functions(sanitized_name.lower())
        
        self.close_header_guard(sanitized_name)
        
        return '\n'.join(self.header_content)
    
    def generate_header_guard(self, name: str) -> None:
        guard_name = f"RISCV_{name.upper()}_GENERATED_H"
        self.header_content.append(f"#ifndef {guard_name}")
        self.header_content.append(f"#define {guard_name}")
        self.header_content.append("")
        self.header_content.append("// Auto-generated from YAML - DO NOT EDIT MANUALLY")
        self.header_content.append("")
        self.header_content.append("#include <stdint.h>")
        self.header_content.append("#include <stdbool.h>")
        self.header_content.append("#include <stddef.h>")
        self.header_content.append("")
    
    def generate_access_functions(self, name: str) -> None:
        self.header_content.append("// Access functions")
        self.header_content.append(f"const void* get_{name}_data(void);")
        self.header_content.append(f"const yaml_field_t* get_yaml_fields(void);")
        self.header_content.append(f"int get_yaml_field_count(void);")
        self.header_content.append("")
        
        self.header_content.append("// Function implementations")
        self.header_content.append(f"const void* get_{name}_data(void) {{")
        self.header_content.append(f"    return &{name}_data;")
        self.header_content.append("}")
        self.header_content.append("")
        
        self.header_content.append(f"const yaml_field_t* get_yaml_fields(void) {{")
        self.header_content.append(f"    return yaml_fields;")
        self.header_content.append("}")
        self.header_content.append("")
        
        self.header_content.append(f"int get_yaml_field_count(void) {{")
        self.header_content.append(f"    return YAML_FIELDS_COUNT;")
        self.header_content.append("}")
        self.header_content.append("")
    
    def close_header_guard(self, name: str) -> None:
        guard_name = f"RISCV_{name.upper()}_GENERATED_H"
        self.header_content.append(f"#endif // {guard_name}")
    
    def save_header_file(self, output_path: str) -> bool:
        try:
            header_content = self.convert_to_c_header()
            with open(output_path, 'w') as file:
                file.write(header_content)
            return True
        except Exception as e:
            print(f"Error saving header file: {e}")
            return False

def main():
    if len(sys.argv) != 3:
        print("Usage: python yaml_to_c.py <input_yaml_file> <output_header_file>")
        print("Example: python yaml_to_c.py jal.yaml jal_generated.h")
        sys.exit(1)
    
    input_yaml = sys.argv[1]
    output_header = sys.argv[2]
    
    if not os.path.exists(input_yaml):
        print(f"Error: Input file '{input_yaml}' does not exist")
        sys.exit(1)
    
    converter = GenericYAMLToCConverter()
    
    print(f"Loading YAML file: {input_yaml}")
    if not converter.load_yaml_file(input_yaml):
        print("Failed to load YAML file")
        sys.exit(1)
    
    print(f"Converting to C header: {output_header}")
    if not converter.save_header_file(output_header):
        print("Failed to save header file")
        sys.exit(1)
    
    print("Conversion completed successfully!")
    print(f"Generated header file: {output_header}")

if __name__ == "__main__":
    main()
