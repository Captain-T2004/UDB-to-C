# RISC-V YAML to C Header Converter

## Overview

1. Reads RISC-V YAML files from the Unified Database
2. Converts YAML → C header → YAML → C header → YAML
3. Validates that the final two YAML outputs match

## Files

- `yaml_to_c.py` - Python script that converts YAML files to C headers
- `yaml_generator.c` - C program that reads generated headers and outputs YAML
- Input YAML files from [RISC-V Unified Database](https://github.com/riscv-software-src/riscv-unified-db)

## Quick Demo

### Prerequisites

```bash
# Ensure you have gcc and Python 3
gcc --version
python3 --version
python3 -m pip install -r pip-requirements.txt
```

### Complete Round-Trip Demonstration

```bash
# Step 1: Get a RISC-V YAML file (example using 'A' extension)
# Download from: https://github.com/riscv-software-src/riscv-unified-db/tree/main/spec/std/isa/ext
# Save as input.yaml

# Step 2: First conversion - YAML to C header
python3 yaml_to_c.py input.yaml test.h

# Step 3: Compile C program with generated header
gcc -o yaml_generator yaml_generator.c -I.

# Step 4: First reconstruction - C header to YAML
./yaml_generator output1.yaml

# Step 5: Second conversion - Generated YAML to C header
python3 yaml_to_c.py output1.yaml test.h

# Step 6: Recompile with new header
gcc -o yaml_generator yaml_generator.c -I.

# Step 7: Second reconstruction - C header to YAML
./yaml_generator output2.yaml

# Step 8: Verify round-trip stability
diff output1.yaml output2.yaml
```