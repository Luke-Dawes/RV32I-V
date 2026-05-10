#pragma once
#include <cstdint>
#include "../Instructions/R-type/R-Instructions.h"
#include "../Instructions/I-type/I-Instructions.h"
#include "../Instructions/S-type/S-Instructions.h"

using std::uint32_t;

//little edian memory 
extern uint8_t RAM[];
extern uint32_t registers[];
extern int32_t PC;

constexpr auto out_of_bounds = 0x1;
constexpr auto miss_aligned_trap = 0x2;

uint32_t fetch(uint32_t PC);

void init_memory();

struct Decoded {
    uint8_t opcode;
    uint8_t funct3;
    uint8_t funct7;
    uint8_t rd, rs1, rs2;
    int32_t imm;
    uint32_t index;
};

Decoded decode(uint32_t CIR);


// ================ bug ===============================
// rd, rs1, rs2 are all uint8_t. I need to change every function to match this and this typedef
typedef void (*InstructionFunc)(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);


extern InstructionFunc Instructions[];

/*
    case 0x33:
        // R-type (Register-Register operations)
        // Example: add, sub, sll, slt, xor, srl, sra, or, and
        printf("Format: R-type\n");
        break;

    case 0x13:
        // I-type (ALU with immediate)
        // Example: addi, slti, xori, ori, andi, slli
        printf("Format: I-type (ALU)\n");
        break;

    case 0x03:
        // I-type (Load instructions)
        // Example: lb, lh, lw, lbu, lhu
        printf("Format: I-type (Load)\n");
        break;

    case 0x67:
        // I-type (JALR)
        printf("Format: I-type (JALR)\n");
        break;

    case 0x23:
        // S-type (Store instructions)
        // Example: sb, sh, sw
        printf("Format: S-type\n");
        break;

    case 0x63:
        // B-type (Branch instructions)
        // Example: beq, bne, blt, bge
        printf("Format: B-type\n");
        break;

    case 0x37:
    case 0x17:
        // U-type (LUI or AUIPC)
        printf("Format: U-type\n");
        break;

    case 0x6F:
        // J-type (Jump and Link)
        // Example: jal
        printf("Format: J-type\n");
        break;

    case 0x73:
        // I-type (System calls/Environment)
        // Example: ecall, ebreak
        printf("Format: System/I-type\n");
        break;

    default:
        printf("Unknown Opcode: 0x%02X\n", opcode);
        break;
    }
}
*/