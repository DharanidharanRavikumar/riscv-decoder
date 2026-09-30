# RISC-V RV32I Instruction Decoder

[![C11](https://img.shields.io/badge/Language-C11-blue.svg)](https://en.wikipedia.org/wiki/C11_(C_specification))
[![Architecture](https://img.shields.io/badge/Architecture-RISC--V%20(RV32I)-red.svg)](https://riscv.org/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

A lightweight, modular, and bit-accurate 32-bit **RISC-V (RV32I Base Integer Instruction Set)** instruction decoder written in C.

This tool disassembles raw 32-bit hexadecimal machine code into its architectural components—including instruction type, mnemonic name, register operands (`rd`, `rs1`, `rs2`), opcode, `funct3`, `funct7`, immediate values (with proper sign extension and non-contiguous bit reconstruction), and shift amounts (`shamt`).

---

## Table of Contents

- [Features](#features)
- [Supported Instruction Set](#supported-instruction-set)
- [RISC-V Instruction Formats](#risc-v-instruction-formats)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Building the Project](#building-the-project)
  - [Running the Decoder](#running-the-decoder)
- [Example Usage & Output](#example-usage--output)
- [API Reference (Library Usage)](#api-reference-library-usage)
- [Test Vectors](#test-vectors)
- [Roadmap](#roadmap)
- [Author](#author)
- [License](#license)

---

## Features

- **Full RV32I Coverage**: Decodes all standard RV32I unprivileged base integer instructions (R, I, S, B, U, and J types).
- **Bit-Accurate Immediate Decoding**: Correctly reconstructs and sign-extends non-contiguous immediate formats:
  - **S-Type**: Split 12-bit signed immediate (`imm[11:5]` and `imm[4:0]`).
  - **B-Type**: Scrambled 13-bit branch offset (`imm[12]`, `imm[11]`, `imm[10:5]`, `imm[4:1]`).
  - **U-Type**: 20-bit upper immediate aligned to bit 12 (`imm[31:12]`).
  - **J-Type**: Scrambled 21-bit jump offset (`imm[20]`, `imm[19:12]`, `imm[11]`, `imm[10:1]`).
- **Clean Decoupled Architecture**: Core decoding logic (`decoder.c`, `decoder.h`) is completely separated from the interactive CLI driver (`main.c`), making it easy to embed into custom CPU emulators, cycle-accurate simulators, or disassemblers.
- **Interactive Terminal REPL**: Fast prompt for testing arbitrary 32-bit hexadecimal instructions in real-time.
- **Comprehensive Verification Suite**: Included reference vectors in `Sample_Inst.txt` for validating instruction decoding.

---

## Supported Instruction Set

The decoder supports 40+ instructions from the **RV32I Base Integer Instruction Set**:

| Format | Category | Instructions Supported |
| :--- | :--- | :--- |
| **R-Type** | Arithmetic & Logical (Reg-Reg) | `ADD`, `SUB`, `SLL`, `SLT`, `SLTU`, `XOR`, `SRL`, `SRA`, `OR`, `AND` |
| **I-Type** | Arithmetic & Logical (Immediate) | `ADDI`, `SLTI`, `SLTIU`, `XORI`, `ORI`, `ANDI`, `SLLI`, `SRLI`, `SRAI` |
| **I-Type** | Loads | `LB`, `LH`, `LW`, `LBU`, `LHU` |
| **I-Type** | Jump & Link Register | `JALR` |
| **S-Type** | Stores | `SB`, `SH`, `SW` |
| **B-Type** | Conditional Branches | `BEQ`, `BNE`, `BLT`, `BGE`, `BLTU`, `BGEU` |
| **U-Type** | Upper Immediate | `LUI`, `AUIPC` |
| **J-Type** | Unconditional Jump | `JAL` |

---

## RISC-V Instruction Formats

The decoder parses raw 32-bit words according to the official RISC-V specification:

```text
         31         25 24   20 19   15 14    12 11        7 6      0
R-Type: |    funct7   |  rs2  |  rs1  | funct3 |    rd     | opcode |
I-Type: |        imm[11:0]    |  rs1  | funct3 |    rd     | opcode |
S-Type: |   imm[11:5] |  rs2  |  rs1  | funct3 | imm[4:0]  | opcode |
B-Type: |12| imm[10:5]|  rs2  |  rs1  | funct3 |imm[4:1]|11| opcode |
U-Type: |                  imm[31:12]          |    rd     | opcode |
J-Type: |20|    imm[10:1]   |11|  imm[19:12]   |    rd     | opcode |
```

---

## Project Structure

```text
riscv-decoder/
├── decoder.h        # Type definitions (enum, struct) and decoder prototypes
├── decoder.c        # Core decoder functions & immediate extraction logic
├── main.c           # Interactive CLI driver application
├── Sample_Inst.txt  # Reference C-array containing test instruction hex codes
├── Makefile         # Build script for gcc
├── .gitignore       # Git ignore rules for build artifacts
└── README.md        # Project documentation
```

---

## Getting Started

### Prerequisites

- A C compiler supporting **C11** (e.g., `gcc` or `clang`).
- `make` (optional, for automated builds).
- Linux, macOS, or Windows (via WSL, MinGW, or MSYS2).

### Building the Project

Clone the repository and compile using `make`:

```bash
git clone https://github.com/DharanidharanRavikumar/riscv-decoder.git
cd riscv-decoder
make
```

Or compile manually using `gcc`:

```bash
gcc -Wall -Wextra -std=c11 main.c decoder.c -o decoder
```

To clean compiled binaries:

```bash
make clean
```

---

## Running the Decoder

Launch the compiled executable:

```bash
./decoder
```

Enter any 32-bit instruction as a hexadecimal value (without leading `0x` when prompted, or enter `0` to quit).

---

## Example Usage & Output

### 1. Decoding an R-Type Instruction (`ADD x0, x6, x3`)
**Input:** `003100B3`

```text
========================================
       RISC-V RV32I INSTRUCTION DECODER
========================================

Enter a 32-bit instruction in hexadecimal.
Example: 0x003100B3
Enter 0 to exit.

Instruction > 0x003100B3

----------------------------------------
           DECODED INSTRUCTION
----------------------------------------
Original Raw instruction : 0x003100B3
Instruction     : ADD
Type            : R
Opcode          : 0x33
rd              : x1
rs1             : x2
rs2             : x3
funct3          : 0x00
funct7          : 0x00
Immediate       : 0
shamt           : 0
----------------------------------------
```

### 2. Decoding an I-Type Instruction (`ADDI`)
**Input:** `00A10093`

```text
Instruction > 0x00A10093

----------------------------------------
           DECODED INSTRUCTION
----------------------------------------
Original Raw instruction : 0x00A10093
Instruction     : ADDI
Type            : I
Opcode          : 0x13
rd              : x1
rs1             : x2
rs2             : x0
funct3          : 0x00
funct7          : 0x00
Immediate       : 10
shamt           : 0
----------------------------------------
```

### 3. Decoding a Branch Instruction (`BEQ`)
**Input:** `00208863`

```text
Instruction > 0x00208863

----------------------------------------
           DECODED INSTRUCTION
----------------------------------------
Original Raw instruction : 0x00208863
Instruction     : BEQ
Type            : B
Opcode          : 0x63
rd              : x0
rs1             : x1
rs2             : x2
funct3          : 0x00
funct7          : 0x00
Immediate       : 16
shamt           : 0
----------------------------------------
```

---

## API Reference (Library Usage)

You can easily embed `decoder.c` and `decoder.h` into your own RISC-V simulator or emulator:

```c
#include "decoder.h"
#include <stdio.h>

int main(void) {
    uint32_t raw_inst = 0x010000EF; // JAL x1, offset
    decode_instruction decoded = decoder(raw_inst);

    printf("Decoded: %s\n", decoded.instruction_name);
    printf("Format : %s-Type\n", type_name(decoded.type));
    printf("Target : rd = x%d, offset = %d\n", decoded.rd, decoded.immediate);

    return 0;
}
```

### Data Structure (`decode_instruction`)

Defined in `decoder.h`:

```c
typedef struct {
    uint32_t og_inp_inst;          // Original 32-bit instruction
    InstructionType type;          // TYPE_R, TYPE_I, TYPE_S, TYPE_B, TYPE_U, TYPE_J
    int32_t immediate;             // Sign-extended reconstructed immediate value
    uint8_t opcode;                // 7-bit opcode field
    uint8_t func3;                 // 3-bit funct3 field
    uint8_t func7;                 // 7-bit funct7 field
    uint8_t rd;                    // Destination register index (0-31)
    uint8_t rs1;                   // Source register 1 index (0-31)
    uint8_t rs2;                   // Source register 2 index (0-31)
    uint8_t shamt;                 // Shift amount for shift instructions (SLLI, SRLI, SRAI)
    const char *instruction_name;  // Mnemonic string (e.g., "ADD", "LW", "BNE")
} decode_instruction;
```

---

## Test Vectors

The file `Sample_Inst.txt` provides verified machine code samples across all categories:

- **R-Type**: `0x003100B3` (ADD), `0x403100B3` (SUB), `0x003110B3` (SLL), `0x003120B3` (SLT)...
- **I-Type Arithmetic**: `0x00A10093` (ADDI), `0x00311093` (SLLI), `0x40315093` (SRAI)...
- **Loads**: `0x00010083` (LB), `0x00012083` (LW), `0x00014083` (LBU)...
- **Stores**: `0x00310023` (SB), `0x00311023` (SH), `0x00312023` (SW)...
- **Branches**: `0x00208863` (BEQ), `0x00209863` (BNE), `0x0020C863` (BLT)...
- **Upper Immediate**: `0x123450B7` (LUI), `0x12345097` (AUIPC)...
- **Jumps**: `0x010000EF` (JAL), `0x000100E7` (JALR)...

---

## Roadmap

- [ ] Full assembly string disassembly output formatting (e.g., `add x1, x2, x3`).
- [ ] Support for **RV32M** (Integer Multiplication and Division) extension.
- [ ] Support for **CSR** (Control and Status Register) instructions (`SYSTEM` opcode `0x73`).
- [ ] Batch file input mode to disassemble ELF / binary / raw hex dump files directly.

---

## Author

- **Dharanidharan R**
- GitHub: [@DharanidharanRavikumar](https://github.com/DharanidharanRavikumar)
- Email: [dharanidharanr12@gmail.com](mailto:dharanidharanr12@gmail.com)

---

## License

This project is open-source and available under the [MIT License](LICENSE).
