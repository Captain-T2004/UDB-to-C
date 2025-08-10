#ifndef RISCV_C_GENERATED_H
#define RISCV_C_GENERATED_H

// Auto-generated from YAML - DO NOT EDIT MANUALLY

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Structure for versions items
typedef struct {
    void* implies;
    const char* ratification_date;
    const char* state;
    const char* version;
} versions_item_t;

// Structure definition for company_t
typedef struct {
    const char* name;
    const char* url;
} company_t;

// Structure definition for doc_license_t
typedef struct {
    const char* name;
    const char* url;
} doc_license_t;

// Structure definition for params_MUTABLE_MISA_C_schema_t
typedef struct {
    const char* type;
} params_MUTABLE_MISA_C_schema_t;

// Structure definition for params_MUTABLE_MISA_C_t
typedef struct {
    const char* description;
    params_MUTABLE_MISA_C_schema_t schema;
} params_MUTABLE_MISA_C_t;

// Structure definition for params_t
typedef struct {
    params_MUTABLE_MISA_C_t MUTABLE_MISA_C;
} params_t;

// Main YAML data structure
typedef struct {
    const char* kind;
    const char* name;
    const char* type;
    const char* long_name;
    company_t company;
    doc_license_t doc_license;
    const void* versions_array;
    int versions_count;
    const char* description;
    params_t params;
} c_data_t;

static const versions_item_t c_versions[] = {
    {
        .version = "2.0.0",
        .state = "ratified",
        .ratification_date = "2019-12",
        .implies = NULL
    }
};
#define C_VERSIONS_COUNT 1

// Main data instance
static const c_data_t c_data = {
    .kind = "extension",
    .name = "C",
    .type = "unprivileged",
    .long_name = "Compressed instructions",
    .company = {
        .name = "RISC-V International",
        .url = "https://riscv.org",
    },
    .doc_license = {
        .name = "Creative Commons Attribution 4.0 International License",
        .url = "https://creativecommons.org/licenses/by/4.0/",
    },
    .versions_array = c_versions,
    .versions_count = 1,
    .description = "The `C` extension reduces static and dynamic code size by\n" \
    "adding short 16-bit instruction encodings for common operations. The C\n" \
    "extension can be added to any of the base ISAs (RV32, RV64, RV128), and\n" \
    "we use the generic term \"RVC\" to cover any of these. Typically,\n" \
    "50%-60% of the RISC-V instructions in a program can be replaced with RVC\n" \
    "instructions, resulting in a 25%-30% code-size reduction.\n" \
    "\n" \
    "= Overview\n" \
    "\n" \
    "RVC uses a simple compression scheme that offers shorter 16-bit versions\n" \
    "of common 32-bit RISC-V instructions when:\n" \
    "\n" \
    "* the immediate or address offset is small, or\n" \
    "* one of the registers is the zero register (`x0`), the ABI link register\n" \
    "(`x1`), or the ABI stack pointer (`x2`), or\n" \
    "* the destination register and the first source register are identical, or\n" \
    "* the registers used are the 8 most popular ones.\n" \
    "\n" \
    "The C extension is compatible with all other standard instruction\n" \
    "extensions. The C extension allows 16-bit instructions to be freely\n" \
    "intermixed with 32-bit instructions, with the latter now able to start\n" \
    "on any 16-bit boundary, i.e., IALIGN=16. With the addition of the C\n" \
    "extension, no instructions can raise instruction-address-misaligned\n" \
    "exceptions.\n" \
    "\n" \
    "[NOTE]\n" \
    "====\n" \
    "Removing the 32-bit alignment constraint on the original 32-bit\n" \
    "instructions allows significantly greater code density.\n" \
    "====\n" \
    "\n" \
    "The compressed instruction encodings are mostly common across RV32C,\n" \
    "RV64C, and RV128C, but as shown in <<rvc-instr-table0, Table 34>>, a few opcodes are used for\n" \
    "different purposes depending on base ISA. For example, the wider\n" \
    "address-space RV64C and RV128C variants require additional opcodes to\n" \
    "compress loads and stores of 64-bit integer values, while RV32C uses the\n" \
    "same opcodes to compress loads and stores of single-precision\n" \
    "floating-point values. Similarly, RV128C requires additional opcodes to\n" \
    "capture loads and stores of 128-bit integer values, while these same\n" \
    "opcodes are used for loads and stores of double-precision floating-point\n" \
    "values in RV32C and RV64C. If the C extension is implemented, the\n" \
    "appropriate compressed floating-point load and store instructions must\n" \
    "be provided whenever the relevant standard floating-point extension (F\n" \
    "and/or D) is also implemented. In addition, RV32C includes a compressed\n" \
    "jump and link instruction to compress short-range subroutine calls,\n" \
    "where the same opcode is used to compress ADDIW for RV64C and RV128C.\n" \
    "\n" \
    "[TIP]\n" \
    "====\n" \
    "Double-precision loads and stores are a significant fraction of static\n" \
    "and dynamic instructions, hence the motivation to include them in the\n" \
    "RV32C and RV64C encoding.\n" \
    "\n" \
    "Although single-precision loads and stores are not a significant source\n" \
    "of static or dynamic compression for benchmarks compiled for the\n" \
    "currently supported ABIs, for microcontrollers that only provide\n" \
    "hardware single-precision floating-point units and have an ABI that only\n" \
    "supports single-precision floating-point numbers, the single-precision\n" \
    "loads and stores will be used at least as frequently as double-precision\n" \
    "loads and stores in the measured benchmarks. Hence, the motivation to\n" \
    "provide compressed support for these in RV32C.\n" \
    "\n" \
    "Short-range subroutine calls are more likely in small binaries for\n" \
    "microcontrollers, hence the motivation to include these in RV32C.\n" \
    "\n" \
    "Although reusing opcodes for different purposes for different base ISAs\n" \
    "adds some complexity to documentation, the impact on implementation\n" \
    "complexity is small even for designs that support multiple base ISAs.\n" \
    "The compressed floating-point load and store variants use the same\n" \
    "instruction format with the same register specifiers as the wider\n" \
    "integer loads and stores.\n" \
    "====\n" \
    "\n" \
    "RVC was designed under the constraint that each RVC instruction expands\n" \
    "into a single 32-bit instruction in either the base ISA (RV32I/E, RV64I/E,\n" \
    "or RV128I) or the F and D standard extensions where present. Adopting\n" \
    "this constraint has two main benefits:\n" \
    "\n" \
    "* Hardware designs can simply expand RVC instructions during decode,\n" \
    "simplifying verification and minimizing modifications to existing\n" \
    "microarchitectures.\n" \
    "\n" \
    "* Compilers can be unaware of the RVC extension and leave code compression\n" \
    "to the assembler and linker, although a compression-aware compiler will\n" \
    "generally be able to produce better results.\n" \
    "\n" \
    "[NOTE]\n" \
    "====\n" \
    "We felt the multiple complexity reductions of a simple one-one mapping\n" \
    "between C and base IFD instructions far outweighed the potential gains\n" \
    "of a slightly denser encoding that added additional instructions only\n" \
    "supported in the C extension, or that allowed encoding of multiple IFD\n" \
    "instructions in one C instruction.\n" \
    "====\n" \
    "\n" \
    "It is important to note that the C extension is not designed to be a\n" \
    "stand-alone ISA, and is meant to be used alongside a base ISA.\n" \
    "\n" \
    "[TIP]\n" \
    "====\n" \
    "Variable-length instruction sets have long been used to improve code\n" \
    "density. For example, the IBM Stretch cite:[stretch], developed in the late 1950s, had\n" \
    "an ISA with 32-bit and 64-bit instructions, where some of the 32-bit\n" \
    "instructions were compressed versions of the full 64-bit instructions.\n" \
    "Stretch also employed the concept of limiting the set of registers that\n" \
    "were addressable in some of the shorter instruction formats, with short\n" \
    "branch instructions that could only refer to one of the index registers.\n" \
    "The later IBM 360 architecture cite:[ibm360] supported a simple variable-length\n" \
    "instruction encoding with 16-bit, 32-bit, or 48-bit instruction formats.\n" \
    "\n" \
    "In 1963, CDC introduced the Cray-designed CDC 6600 cite:[cdc6600], a precursor to RISC\n" \
    "architectures, that introduced a register-rich load-store architecture\n" \
    "with instructions of two lengths, 15-bits and 30-bits. The later Cray-1\n" \
    "design used a very similar instruction format, with 16-bit and 32-bit\n" \
    "instruction lengths.\n" \
    "\n" \
    "The initial RISC ISAs from the 1980s all picked performance over code\n" \
    "size, which was reasonable for a workstation environment, but not for\n" \
    "embedded systems. Hence, both ARM and MIPS subsequently made versions of\n" \
    "the ISAs that offered smaller code size by offering an alternative\n" \
    "16-bit wide instruction set instead of the standard 32-bit wide\n" \
    "instructions. The compressed RISC ISAs reduced code size relative to\n" \
    "their starting points by about 25-30%, yielding code that was\n" \
    "significantly smaller than 80x86. This result surprised some, as their\n" \
    "intuition was that the variable-length CISC ISA should be smaller than\n" \
    "RISC ISAs that offered only 16-bit and 32-bit formats.\n" \
    "\n" \
    "Since the original RISC ISAs did not leave sufficient opcode space free\n" \
    "to include these unplanned compressed instructions, they were instead\n" \
    "developed as complete new ISAs. This meant compilers needed different\n" \
    "code generators for the separate compressed ISAs. The first compressed\n" \
    "RISC ISA extensions (e.g., ARM Thumb and MIPS16) used only a fixed\n" \
    "16-bit instruction size, which gave good reductions in static code size\n" \
    "but caused an increase in dynamic instruction count, which led to lower\n" \
    "performance compared to the original fixed-width 32-bit instruction\n" \
    "size. This led to the development of a second generation of compressed\n" \
    "RISC ISA designs with mixed 16-bit and 32-bit instruction lengths (e.g.,\n" \
    "ARM Thumb2, microMIPS, PowerPC VLE), so that performance was similar to\n" \
    "pure 32-bit instructions but with significant code size savings.\n" \
    "Unfortunately, these different generations of compressed ISAs are\n" \
    "incompatible with each other and with the original uncompressed ISA,\n" \
    "leading to significant complexity in documentation, implementations, and\n" \
    "software tools support.\n" \
    "\n" \
    "Of the commonly used 64-bit ISAs, only PowerPC and microMIPS currently\n" \
    "supports a compressed instruction format. It is surprising that the most\n" \
    "popular 64-bit ISA for mobile platforms (ARM v8) does not include a\n" \
    "compressed instruction format given that static code size and dynamic\n" \
    "instruction fetch bandwidth are important metrics. Although static code\n" \
    "size is not a major concern in larger systems, instruction fetch\n" \
    "bandwidth can be a major bottleneck in servers running commercial\n" \
    "workloads, which often have a large instruction working set.\n" \
    "\n" \
    "Benefiting from 25 years of hindsight, RISC-V was designed to support\n" \
    "compressed instructions from the outset, leaving enough opcode space for\n" \
    "RVC to be added as a simple extension on top of the base ISA (along with\n" \
    "many other extensions). The philosophy of RVC is to reduce code size for\n" \
    "embedded applications _and_ to improve performance and energy-efficiency\n" \
    "for all applications due to fewer misses in the instruction cache.\n" \
    "Waterman shows that RVC fetches 25%-30% fewer instruction bits, which\n" \
    "reduces instruction cache misses by 20%-25%, or roughly the same\n" \
    "performance impact as doubling the instruction cache size. cite:[waterman-ms]\n" \
    "====\n" \
    "\n" \
    "= Compressed Instruction Formats\n" \
    "((((compressed, formats))))\n" \
    "\n" \
    "<<rvc-form>> shows the nine compressed instruction\n" \
    "formats. CR, CI, and CSS can use any of the 32 RVI registers, but CIW,\n" \
    "CL, CS, CA, and CB are limited to just 8 of them.\n" \
    "<<registers>> lists these popular registers, which\n" \
    "correspond to registers `x8` to `x15`. Note that there is a separate\n" \
    "version of load and store instructions that use the stack pointer as the\n" \
    "base address register, since saving to and restoring from the stack are\n" \
    "so prevalent, and that they use the CI and CSS formats to allow access\n" \
    "to all 32 data registers. CIW supplies an 8-bit immediate for the\n" \
    "ADDI4SPN instruction.\n" \
    "\n" \
    "[NOTE]\n" \
    "====\n" \
    "The RISC-V ABI was changed to make the frequently used registers map to\n" \
    "registers 'x8-x15'. This simplifies the decompression decoder by\n" \
    "having a contiguous naturally aligned set of register numbers, and is\n" \
    "also compatible with the RV32E and RV64E base ISAs, which only have 16 integer\n" \
    "registers.\n" \
    "====\n" \
    "Compressed register-based floating-point loads and stores also use the\n" \
    "CL and CS formats respectively, with the eight registers mapping to `f8` to `f15`.\n" \
    "((((calling convention, standard))))\n" \
    "[NOTE]\n" \
    "====\n" \
    "_The standard RISC-V calling convention maps the most frequently used\n" \
    "floating-point registers to registers `f8` to `f15`, which allows the\n" \
    "same register decompression decoding as for integer register numbers._\n" \
    "====\n" \
    "((((register source specifiers, c-ext))))\n" \
    "The formats were designed to keep bits for the two register source\n" \
    "specifiers in the same place in all instructions, while the destination\n" \
    "register field can move. When the full 5-bit destination register\n" \
    "specifier is present, it is in the same place as in the 32-bit RISC-V\n" \
    "encoding. Where immediates are sign-extended, the sign extension is\n" \
    "always from bit 12. Immediate fields have been scrambled, as in the base\n" \
    "specification, to reduce the number of immediate muxes required.\n" \
    "[NOTE]\n" \
    "====\n" \
    "The immediate fields are scrambled in the instruction formats instead of\n" \
    "in sequential order so that as many bits as possible are in the same\n" \
    "position in every instruction, thereby simplifying implementations.\n" \
    "====\n" \
    "\n" \
    "For many RVC instructions, zero-valued immediates are disallowed and\n" \
    "`x0` is not a valid 5-bit register specifier. These restrictions free up\n" \
    "encoding space for other instructions requiring fewer operand bits.\n" \
    "\n" \
    "//[[cr-register]]\n" \
    "//include::images/wavedrom/cr-register.adoc[]\n" \
    "//.Compressed 16-bit RVC instructions\n" \
    "//(((compressed, 16-bit)))\n" \
    "\n" \
    "[[rvc-form]]\n" \
    ".Compressed 16-bit RVC instruction formats\n" \
    "//[%header]\n" \
    "[float=\"center\",align=\"center\",cols=\"1a, 2a\",frame=\"none\",grid=\"none\"]\n" \
    "|===\n" \
    "|\n" \
    "[%autowidth,float=\"right\",align=\"right\",cols=\"^,^\",frame=\"none\",grid=\"none\",options=\"noheader\"]\n" \
    "!===\n" \
    "!Format ! Meaning\n" \
    "!CR ! Register\n" \
    "!CI ! Immediate\n" \
    "!CSS ! Stack-relative Store\n" \
    "!CIW ! Wide Immediate\n" \
    "!CL ! Load\n" \
    "!CS ! Store\n" \
    "!CA ! Arithmetic\n" \
    "!CB ! Branch/Arithmetic\n" \
    "!CJ ! Jump\n" \
    "!===\n" \
    "|\n" \
    "[float=\"left\",align=\"left\",cols=\"1,1,1,1,1,1,1\",options=\"noheader\"]\n" \
    "!===\n" \
    "2+^!15 14 13 12 2+^!11 10 9 8 7 2+^!6 5 4 3 2 ^!1 0\n" \
    "2+^!funct4 2+^!rd/rs1 2+^!rs2 ^!  op\n" \
    "^!funct3 ^!imm 2+^!rd/rs1  2+^!imm ^!  op\n" \
    "^!funct3 3+^!imm  2+^!rs2 ^!  op\n" \
    "^!funct3 4+^!imm ^!rd&#x2032; ^! op\n" \
    "^!funct3 2+^!imm ^!rs1&#x2032; ^!imm ^!rd&#x2032; ^! op\n" \
    "^!funct3 2+^!imm ^!rs1&#x2032; ^! imm ^!rs2&#x2032; ^! op\n" \
    "3+^!funct6 ^!rd&#x2032;/rs1&#x2032; ^!funct2 ^!rs2&#x2032; ^! op\n" \
    "^!funct3 2+^!offset ^!rd&#x2032;/rs1&#x2032; 2+^!offset ^! op\n" \
    "^!funct3 5+^!jump target ^! op\n" \
    "!===\n" \
    "|===\n" \
    "\n" \
    "[[registers]]\n" \
    ".Registers specified by the three-bit _rs1_&#x2032;, _rs2_&#x2032;, and _rd_&#x2032; fields of the CIW, CL, CS, CA, and CB formats.\n" \
    "//[cols=\"20%,10%,10%,10%,10%,10%,10%,10%,10%\"]\n" \
    "[float=\"center\",align=\"center\",cols=\"1a, 1a\",frame=\"none\",grid=\"none\"]\n" \
    "|===\n" \
    "|\n" \
    "[%autowidth,cols=\"<\",frame=\"none\",grid=\"none\",options=\"noheader\"]\n" \
    "!===\n" \
    "!RVC Register Number\n" \
    "!Integer Register Number\n" \
    "!Integer Register ABI Name\n" \
    "!Floating-Point Register Number\n" \
    "!Floating-Point Register ABI Name\n" \
    "!===\n" \
    "|\n" \
    "\n" \
    "[%autowidth,cols=\"^,^,^,^,^,^,^,^\",options=\"noheader\"]\n" \
    "!===\n" \
    "!`000` !`001` !`010` !`011` !`100` !`101` !`110` !`111`\n" \
    "!`x8` !`x9` !`x10` !`x11` !`x12` !`x13` !`x14`!`x15`\n" \
    "!`s0` !`s1` !`a0` !`a1` !`a2` !`a3` !`a4`!`a5`\n" \
    "!`f8` !`f9` !`f10` !`f11` !`f12` !`f13`!`f14` !`f15`\n" \
    "!`fs0` !`fs1` !`fa0` !`fa1` !`fa2`!`fa3` !`fa4` !`fa5`\n" \
    "!===\n" \
    "|===\n" \
    "\n",
    .params = {
        .MUTABLE_MISA_C = {
            .description = "Indicates whether or not the `C` extension can be disabled with the `misa.C` bit.\n" \
    "\n",
            .schema = {
                .type = "boolean",
            },
        },
    },
};

// Comprehensive YAML field reconstruction
typedef struct {
    const char* key;
    const char* type;
    const char* value;
} yaml_field_t;

static const yaml_field_t yaml_fields[] = {
    {"kind", "string", "extension"},
    {"name", "string", "C"},
    {"type", "string", "unprivileged"},
    {"long_name", "string", "Compressed instructions"},
    {"company.name", "string", "RISC-V International"},
    {"company.url", "string", "https://riscv.org"},
    {"doc_license.name", "string", "Creative Commons Attribution 4.0 International License"},
    {"doc_license.url", "string", "https://creativecommons.org/licenses/by/4.0/"},
    {"versions[0].version", "string", "2.0.0"},
    {"versions[0].state", "string", "ratified"},
    {"versions[0].ratification_date", "string", "2019-12"},
    {"versions[0].implies[0].name", "string", "Zca"},
    {"versions[0].implies[0].version", "string", "1.0.0"},
    {"versions[0].implies[1].if.name", "string", "F"},
    {"versions[0].implies[1].if.version", "string", "~> 2.2"},
    {"versions[0].implies[1].then.name", "string", "Zcf"},
    {"versions[0].implies[1].then.version", "string", "1.0.0"},
    {"versions[0].implies[2].if.name", "string", "D"},
    {"versions[0].implies[2].if.version", "string", "~> 2.2"},
    {"versions[0].implies[2].then.name", "string", "Zcd"},
    {"versions[0].implies[2].then.version", "string", "1.0.0"},
    {"description", "string", "The `C` extension reduces static and dynamic code size by\n" \
    "adding short 16-bit instruction encodings for common operations. The C\n" \
    "extension can be added to any of the base ISAs (RV32, RV64, RV128), and\n" \
    "we use the generic term \"RVC\" to cover any of these. Typically,\n" \
    "50%-60% of the RISC-V instructions in a program can be replaced with RVC\n" \
    "instructions, resulting in a 25%-30% code-size reduction.\n" \
    "\n" \
    "= Overview\n" \
    "\n" \
    "RVC uses a simple compression scheme that offers shorter 16-bit versions\n" \
    "of common 32-bit RISC-V instructions when:\n" \
    "\n" \
    "* the immediate or address offset is small, or\n" \
    "* one of the registers is the zero register (`x0`), the ABI link register\n" \
    "(`x1`), or the ABI stack pointer (`x2`), or\n" \
    "* the destination register and the first source register are identical, or\n" \
    "* the registers used are the 8 most popular ones.\n" \
    "\n" \
    "The C extension is compatible with all other standard instruction\n" \
    "extensions. The C extension allows 16-bit instructions to be freely\n" \
    "intermixed with 32-bit instructions, with the latter now able to start\n" \
    "on any 16-bit boundary, i.e., IALIGN=16. With the addition of the C\n" \
    "extension, no instructions can raise instruction-address-misaligned\n" \
    "exceptions.\n" \
    "\n" \
    "[NOTE]\n" \
    "====\n" \
    "Removing the 32-bit alignment constraint on the original 32-bit\n" \
    "instructions allows significantly greater code density.\n" \
    "====\n" \
    "\n" \
    "The compressed instruction encodings are mostly common across RV32C,\n" \
    "RV64C, and RV128C, but as shown in <<rvc-instr-table0, Table 34>>, a few opcodes are used for\n" \
    "different purposes depending on base ISA. For example, the wider\n" \
    "address-space RV64C and RV128C variants require additional opcodes to\n" \
    "compress loads and stores of 64-bit integer values, while RV32C uses the\n" \
    "same opcodes to compress loads and stores of single-precision\n" \
    "floating-point values. Similarly, RV128C requires additional opcodes to\n" \
    "capture loads and stores of 128-bit integer values, while these same\n" \
    "opcodes are used for loads and stores of double-precision floating-point\n" \
    "values in RV32C and RV64C. If the C extension is implemented, the\n" \
    "appropriate compressed floating-point load and store instructions must\n" \
    "be provided whenever the relevant standard floating-point extension (F\n" \
    "and/or D) is also implemented. In addition, RV32C includes a compressed\n" \
    "jump and link instruction to compress short-range subroutine calls,\n" \
    "where the same opcode is used to compress ADDIW for RV64C and RV128C.\n" \
    "\n" \
    "[TIP]\n" \
    "====\n" \
    "Double-precision loads and stores are a significant fraction of static\n" \
    "and dynamic instructions, hence the motivation to include them in the\n" \
    "RV32C and RV64C encoding.\n" \
    "\n" \
    "Although single-precision loads and stores are not a significant source\n" \
    "of static or dynamic compression for benchmarks compiled for the\n" \
    "currently supported ABIs, for microcontrollers that only provide\n" \
    "hardware single-precision floating-point units and have an ABI that only\n" \
    "supports single-precision floating-point numbers, the single-precision\n" \
    "loads and stores will be used at least as frequently as double-precision\n" \
    "loads and stores in the measured benchmarks. Hence, the motivation to\n" \
    "provide compressed support for these in RV32C.\n" \
    "\n" \
    "Short-range subroutine calls are more likely in small binaries for\n" \
    "microcontrollers, hence the motivation to include these in RV32C.\n" \
    "\n" \
    "Although reusing opcodes for different purposes for different base ISAs\n" \
    "adds some complexity to documentation, the impact on implementation\n" \
    "complexity is small even for designs that support multiple base ISAs.\n" \
    "The compressed floating-point load and store variants use the same\n" \
    "instruction format with the same register specifiers as the wider\n" \
    "integer loads and stores.\n" \
    "====\n" \
    "\n" \
    "RVC was designed under the constraint that each RVC instruction expands\n" \
    "into a single 32-bit instruction in either the base ISA (RV32I/E, RV64I/E,\n" \
    "or RV128I) or the F and D standard extensions where present. Adopting\n" \
    "this constraint has two main benefits:\n" \
    "\n" \
    "* Hardware designs can simply expand RVC instructions during decode,\n" \
    "simplifying verification and minimizing modifications to existing\n" \
    "microarchitectures.\n" \
    "\n" \
    "* Compilers can be unaware of the RVC extension and leave code compression\n" \
    "to the assembler and linker, although a compression-aware compiler will\n" \
    "generally be able to produce better results.\n" \
    "\n" \
    "[NOTE]\n" \
    "====\n" \
    "We felt the multiple complexity reductions of a simple one-one mapping\n" \
    "between C and base IFD instructions far outweighed the potential gains\n" \
    "of a slightly denser encoding that added additional instructions only\n" \
    "supported in the C extension, or that allowed encoding of multiple IFD\n" \
    "instructions in one C instruction.\n" \
    "====\n" \
    "\n" \
    "It is important to note that the C extension is not designed to be a\n" \
    "stand-alone ISA, and is meant to be used alongside a base ISA.\n" \
    "\n" \
    "[TIP]\n" \
    "====\n" \
    "Variable-length instruction sets have long been used to improve code\n" \
    "density. For example, the IBM Stretch cite:[stretch], developed in the late 1950s, had\n" \
    "an ISA with 32-bit and 64-bit instructions, where some of the 32-bit\n" \
    "instructions were compressed versions of the full 64-bit instructions.\n" \
    "Stretch also employed the concept of limiting the set of registers that\n" \
    "were addressable in some of the shorter instruction formats, with short\n" \
    "branch instructions that could only refer to one of the index registers.\n" \
    "The later IBM 360 architecture cite:[ibm360] supported a simple variable-length\n" \
    "instruction encoding with 16-bit, 32-bit, or 48-bit instruction formats.\n" \
    "\n" \
    "In 1963, CDC introduced the Cray-designed CDC 6600 cite:[cdc6600], a precursor to RISC\n" \
    "architectures, that introduced a register-rich load-store architecture\n" \
    "with instructions of two lengths, 15-bits and 30-bits. The later Cray-1\n" \
    "design used a very similar instruction format, with 16-bit and 32-bit\n" \
    "instruction lengths.\n" \
    "\n" \
    "The initial RISC ISAs from the 1980s all picked performance over code\n" \
    "size, which was reasonable for a workstation environment, but not for\n" \
    "embedded systems. Hence, both ARM and MIPS subsequently made versions of\n" \
    "the ISAs that offered smaller code size by offering an alternative\n" \
    "16-bit wide instruction set instead of the standard 32-bit wide\n" \
    "instructions. The compressed RISC ISAs reduced code size relative to\n" \
    "their starting points by about 25-30%, yielding code that was\n" \
    "significantly smaller than 80x86. This result surprised some, as their\n" \
    "intuition was that the variable-length CISC ISA should be smaller than\n" \
    "RISC ISAs that offered only 16-bit and 32-bit formats.\n" \
    "\n" \
    "Since the original RISC ISAs did not leave sufficient opcode space free\n" \
    "to include these unplanned compressed instructions, they were instead\n" \
    "developed as complete new ISAs. This meant compilers needed different\n" \
    "code generators for the separate compressed ISAs. The first compressed\n" \
    "RISC ISA extensions (e.g., ARM Thumb and MIPS16) used only a fixed\n" \
    "16-bit instruction size, which gave good reductions in static code size\n" \
    "but caused an increase in dynamic instruction count, which led to lower\n" \
    "performance compared to the original fixed-width 32-bit instruction\n" \
    "size. This led to the development of a second generation of compressed\n" \
    "RISC ISA designs with mixed 16-bit and 32-bit instruction lengths (e.g.,\n" \
    "ARM Thumb2, microMIPS, PowerPC VLE), so that performance was similar to\n" \
    "pure 32-bit instructions but with significant code size savings.\n" \
    "Unfortunately, these different generations of compressed ISAs are\n" \
    "incompatible with each other and with the original uncompressed ISA,\n" \
    "leading to significant complexity in documentation, implementations, and\n" \
    "software tools support.\n" \
    "\n" \
    "Of the commonly used 64-bit ISAs, only PowerPC and microMIPS currently\n" \
    "supports a compressed instruction format. It is surprising that the most\n" \
    "popular 64-bit ISA for mobile platforms (ARM v8) does not include a\n" \
    "compressed instruction format given that static code size and dynamic\n" \
    "instruction fetch bandwidth are important metrics. Although static code\n" \
    "size is not a major concern in larger systems, instruction fetch\n" \
    "bandwidth can be a major bottleneck in servers running commercial\n" \
    "workloads, which often have a large instruction working set.\n" \
    "\n" \
    "Benefiting from 25 years of hindsight, RISC-V was designed to support\n" \
    "compressed instructions from the outset, leaving enough opcode space for\n" \
    "RVC to be added as a simple extension on top of the base ISA (along with\n" \
    "many other extensions). The philosophy of RVC is to reduce code size for\n" \
    "embedded applications _and_ to improve performance and energy-efficiency\n" \
    "for all applications due to fewer misses in the instruction cache.\n" \
    "Waterman shows that RVC fetches 25%-30% fewer instruction bits, which\n" \
    "reduces instruction cache misses by 20%-25%, or roughly the same\n" \
    "performance impact as doubling the instruction cache size. cite:[waterman-ms]\n" \
    "====\n" \
    "\n" \
    "= Compressed Instruction Formats\n" \
    "((((compressed, formats))))\n" \
    "\n" \
    "<<rvc-form>> shows the nine compressed instruction\n" \
    "formats. CR, CI, and CSS can use any of the 32 RVI registers, but CIW,\n" \
    "CL, CS, CA, and CB are limited to just 8 of them.\n" \
    "<<registers>> lists these popular registers, which\n" \
    "correspond to registers `x8` to `x15`. Note that there is a separate\n" \
    "version of load and store instructions that use the stack pointer as the\n" \
    "base address register, since saving to and restoring from the stack are\n" \
    "so prevalent, and that they use the CI and CSS formats to allow access\n" \
    "to all 32 data registers. CIW supplies an 8-bit immediate for the\n" \
    "ADDI4SPN instruction.\n" \
    "\n" \
    "[NOTE]\n" \
    "====\n" \
    "The RISC-V ABI was changed to make the frequently used registers map to\n" \
    "registers 'x8-x15'. This simplifies the decompression decoder by\n" \
    "having a contiguous naturally aligned set of register numbers, and is\n" \
    "also compatible with the RV32E and RV64E base ISAs, which only have 16 integer\n" \
    "registers.\n" \
    "====\n" \
    "Compressed register-based floating-point loads and stores also use the\n" \
    "CL and CS formats respectively, with the eight registers mapping to `f8` to `f15`.\n" \
    "((((calling convention, standard))))\n" \
    "[NOTE]\n" \
    "====\n" \
    "_The standard RISC-V calling convention maps the most frequently used\n" \
    "floating-point registers to registers `f8` to `f15`, which allows the\n" \
    "same register decompression decoding as for integer register numbers._\n" \
    "====\n" \
    "((((register source specifiers, c-ext))))\n" \
    "The formats were designed to keep bits for the two register source\n" \
    "specifiers in the same place in all instructions, while the destination\n" \
    "register field can move. When the full 5-bit destination register\n" \
    "specifier is present, it is in the same place as in the 32-bit RISC-V\n" \
    "encoding. Where immediates are sign-extended, the sign extension is\n" \
    "always from bit 12. Immediate fields have been scrambled, as in the base\n" \
    "specification, to reduce the number of immediate muxes required.\n" \
    "[NOTE]\n" \
    "====\n" \
    "The immediate fields are scrambled in the instruction formats instead of\n" \
    "in sequential order so that as many bits as possible are in the same\n" \
    "position in every instruction, thereby simplifying implementations.\n" \
    "====\n" \
    "\n" \
    "For many RVC instructions, zero-valued immediates are disallowed and\n" \
    "`x0` is not a valid 5-bit register specifier. These restrictions free up\n" \
    "encoding space for other instructions requiring fewer operand bits.\n" \
    "\n" \
    "//[[cr-register]]\n" \
    "//include::images/wavedrom/cr-register.adoc[]\n" \
    "//.Compressed 16-bit RVC instructions\n" \
    "//(((compressed, 16-bit)))\n" \
    "\n" \
    "[[rvc-form]]\n" \
    ".Compressed 16-bit RVC instruction formats\n" \
    "//[%header]\n" \
    "[float=\"center\",align=\"center\",cols=\"1a, 2a\",frame=\"none\",grid=\"none\"]\n" \
    "|===\n" \
    "|\n" \
    "[%autowidth,float=\"right\",align=\"right\",cols=\"^,^\",frame=\"none\",grid=\"none\",options=\"noheader\"]\n" \
    "!===\n" \
    "!Format ! Meaning\n" \
    "!CR ! Register\n" \
    "!CI ! Immediate\n" \
    "!CSS ! Stack-relative Store\n" \
    "!CIW ! Wide Immediate\n" \
    "!CL ! Load\n" \
    "!CS ! Store\n" \
    "!CA ! Arithmetic\n" \
    "!CB ! Branch/Arithmetic\n" \
    "!CJ ! Jump\n" \
    "!===\n" \
    "|\n" \
    "[float=\"left\",align=\"left\",cols=\"1,1,1,1,1,1,1\",options=\"noheader\"]\n" \
    "!===\n" \
    "2+^!15 14 13 12 2+^!11 10 9 8 7 2+^!6 5 4 3 2 ^!1 0\n" \
    "2+^!funct4 2+^!rd/rs1 2+^!rs2 ^!  op\n" \
    "^!funct3 ^!imm 2+^!rd/rs1  2+^!imm ^!  op\n" \
    "^!funct3 3+^!imm  2+^!rs2 ^!  op\n" \
    "^!funct3 4+^!imm ^!rd&#x2032; ^! op\n" \
    "^!funct3 2+^!imm ^!rs1&#x2032; ^!imm ^!rd&#x2032; ^! op\n" \
    "^!funct3 2+^!imm ^!rs1&#x2032; ^! imm ^!rs2&#x2032; ^! op\n" \
    "3+^!funct6 ^!rd&#x2032;/rs1&#x2032; ^!funct2 ^!rs2&#x2032; ^! op\n" \
    "^!funct3 2+^!offset ^!rd&#x2032;/rs1&#x2032; 2+^!offset ^! op\n" \
    "^!funct3 5+^!jump target ^! op\n" \
    "!===\n" \
    "|===\n" \
    "\n" \
    "[[registers]]\n" \
    ".Registers specified by the three-bit _rs1_&#x2032;, _rs2_&#x2032;, and _rd_&#x2032; fields of the CIW, CL, CS, CA, and CB formats.\n" \
    "//[cols=\"20%,10%,10%,10%,10%,10%,10%,10%,10%\"]\n" \
    "[float=\"center\",align=\"center\",cols=\"1a, 1a\",frame=\"none\",grid=\"none\"]\n" \
    "|===\n" \
    "|\n" \
    "[%autowidth,cols=\"<\",frame=\"none\",grid=\"none\",options=\"noheader\"]\n" \
    "!===\n" \
    "!RVC Register Number\n" \
    "!Integer Register Number\n" \
    "!Integer Register ABI Name\n" \
    "!Floating-Point Register Number\n" \
    "!Floating-Point Register ABI Name\n" \
    "!===\n" \
    "|\n" \
    "\n" \
    "[%autowidth,cols=\"^,^,^,^,^,^,^,^\",options=\"noheader\"]\n" \
    "!===\n" \
    "!`000` !`001` !`010` !`011` !`100` !`101` !`110` !`111`\n" \
    "!`x8` !`x9` !`x10` !`x11` !`x12` !`x13` !`x14`!`x15`\n" \
    "!`s0` !`s1` !`a0` !`a1` !`a2` !`a3` !`a4`!`a5`\n" \
    "!`f8` !`f9` !`f10` !`f11` !`f12` !`f13`!`f14` !`f15`\n" \
    "!`fs0` !`fs1` !`fa0` !`fa1` !`fa2`!`fa3` !`fa4` !`fa5`\n" \
    "!===\n" \
    "|===\n" \
    "\n"},
    {"params.MUTABLE_MISA_C.description", "string", "Indicates whether or not the `C` extension can be disabled with the `misa.C` bit.\n" \
    "\n"},
    {"params.MUTABLE_MISA_C.schema.type", "string", "boolean"}
};
#define YAML_FIELDS_COUNT 24

// Access functions
const void* get_c_data(void);
const yaml_field_t* get_yaml_fields(void);
int get_yaml_field_count(void);

// Function implementations
const void* get_c_data(void) {
    return &c_data;
}

const yaml_field_t* get_yaml_fields(void) {
    return yaml_fields;
}

int get_yaml_field_count(void) {
    return YAML_FIELDS_COUNT;
}

#endif // RISCV_C_GENERATED_H