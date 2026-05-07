#pragma once
#include <cstdint>
#include "../Instructions/R-type/R-Instructions.h"
#include "../Instructions/I-type/I-Instructions.h"
#include "../Instructions/S-type/S-Instructions.h"

using std::uint32_t;

//little edian memory 
uint8_t RAM[0x100];
uint32_t registers[31];
int32_t PC = 0;

constexpr auto out_of_bounds = 0x1;
constexpr auto miss_aligned_trap = 0x2;


uint32_t fetch(uint32_t PC) {
	if (PC + 3 >= 0x100) {
		//throw error
		throw out_of_bounds; //this is terminal so i dont think it can catch
	}

	if (PC % 4 != 0) {
		throw miss_aligned_trap;
	}
	

	uint32_t data = RAM[PC] |
		(RAM[PC + 1] << 8) |
		(RAM[PC + 2] << 16) |
		(RAM[PC + 3] << 24);

	return data;
}

struct Decoded {
    uint8_t opcode;
    uint8_t funct3;
    uint8_t funct7;
    uint8_t rd, rs1, rs2;
    int32_t imm;
    uint32_t index;
    uint32_t raw;
};

Decoded decode(uint32_t CIR) {
    return Decoded{
        .opcode = static_cast<uint8_t>(CIR & 0x7F),          // bits 0–6
        .funct3 = static_cast<uint8_t>((CIR >> 12) & 0x7),   // bits 12–14
        .funct7 = static_cast<uint8_t>((CIR >> 25) & 0x7F),  // bits 25–31
        .rd = static_cast<uint8_t>((CIR >> 7) & 0x1F),   // bits 7–11
        .rs1 = static_cast<uint8_t>((CIR >> 15) & 0x1F),  // bits 15–19
        .rs2 = static_cast<uint8_t>((CIR >> 20) & 0x1F),   // bits 20–24
        .imm = static_cast<uint16_t>((CIR >> 20) & 0xFFF), 
        .index = (((CIR >> 30) & 0x1) << 10) | (((CIR >> 12) & 0x7) << 7) | (CIR & 0x7F)
    };
}


typedef void (*InstructionFunc)(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
#define IDX(b30, f3, op) (((b30) << 10) | ((f3) << 7) | (op))

InstructionFunc Instructions[2048] = { nullptr };

void init_table() {
    Instructions[IDX(0, 0, 0x03)] = lb;
    Instructions[IDX(0, 1, 0x03)] = lh;
    Instructions[IDX(0, 2, 0x03)] = lw;
    Instructions[IDX(0, 4, 0x03)] = lbu;
    Instructions[IDX(0, 5, 0x03)] = lhu;

    // --- STORE (Opcode 0x23) ---
    Instructions[IDX(0, 0, 0x23)] = sb;
    Instructions[IDX(0, 1, 0x23)] = sh;
    Instructions[IDX(0, 2, 0x23)] = sw;

    // --- INTEGER REG-IMM (Opcode 0x13) ---
    Instructions[IDX(0, 0, 0x13)] = addi;
    Instructions[IDX(0, 2, 0x13)] = slti;
    Instructions[IDX(0, 3, 0x13)] = sltiu;
    Instructions[IDX(0, 4, 0x13)] = xori;
    Instructions[IDX(0, 6, 0x13)] = ori;
    Instructions[IDX(0, 7, 0x13)] = andi;
    Instructions[IDX(0, 1, 0x13)] = slli;
    Instructions[IDX(0, 5, 0x13)] = srli;
    Instructions[IDX(1, 5, 0x13)] = srai; 

    // --- INTEGER REG-REG (Opcode 0x33) ---
    Instructions[IDX(0, 0, 0x33)] = add;
    Instructions[IDX(1, 0, 0x33)] = sub; 
    Instructions[IDX(0, 1, 0x33)] = sll;
    Instructions[IDX(0, 2, 0x33)] = slt;
    Instructions[IDX(0, 3, 0x33)] = sltu;
    Instructions[IDX(0, 4, 0x33)] = _xor;
    Instructions[IDX(0, 5, 0x33)] = srl;
    Instructions[IDX(1, 5, 0x33)] = sra; 
    Instructions[IDX(0, 6, 0x33)] = _or;
    Instructions[IDX(0, 7, 0x33)] = _and;

    // --- BRANCH (Opcode 0x63) ---
    Instructions[IDX(0, 0, 0x63)] = beq;
    Instructions[IDX(0, 1, 0x63)] = bne;
    Instructions[IDX(0, 4, 0x63)] = blt;
    Instructions[IDX(0, 5, 0x63)] = bge;
    Instructions[IDX(0, 6, 0x63)] = bltu;
    Instructions[IDX(0, 7, 0x63)] = bgeu;

    // --- JUMP & UPPER IMM (Various Opcodes) ---
    Instructions[IDX(0, 0, 0x6F)] = jal;   // Opcode 0x6F
    Instructions[IDX(0, 0, 0x67)] = jalr;  // Opcode 0x67 (funct3=0)
    Instructions[IDX(0, 0, 0x37)] = lui;   // Opcode 0x37
    Instructions[IDX(0, 0, 0x17)] = auipc; // Opcode 0x17

    // --- SYSTEM (Opcode 0x73) ---
    Instructions[IDX(0, 0, 0x73)] = system_call; // Handles ECALL/EBREAK
}



void run_instructions(uint32_t CIR) {
    Decoded current = decode(CIR);

    switch (current.opcode) {

        case 0x33: //R-type

            switch (current.funct3) {

                case 0x0:
                    //add or sub
                    (current.funct7 == 0x20) ? sub(current.rd, current.rs1, current.rs2) : add(current.rd, current.rs1,current.rs2);

                    break;
                case 0x1:
                    sll(current.rd, current.rs1, current.rs2);
                    //logical left shift 
                    break;

                case 0x2:
                    slt(current.rd, current.rs1, current.rs2);
                    //set less than
                    break;

                case 0x3:
                    sltu(current.rd, current.rs1, current.rs2);
                    //set less than (u)
                    break;

                case 0x4:
                    _xor(current.rd, current.rs1, current.rs2);
                    //xor
                    break;

                case 0x5:
                    //shift right logical
                    //shift right arith

                    (current.funct7 == 0x20) ? srl(current.rd, current.rs1, current.rs2) : sra(current.rd, current.rs1, current.rs2);
                    
                    break;

                case 0x6:
                    //or
                    _or(current.rd, current.rs1, current.rs2);
                    break;

                case 0x7:
                    //and
                    _and(current.rd, current.rs1, current.rs2);
                    break;

                default:
                    //broken
                    break;
            }

            break;
        
        case 0x13: //I-type

            break;

        default:
            break;

}


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