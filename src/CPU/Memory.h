#pragma once
#include <cstdint>
#include "../Instructions/R-type/Instructions.h"

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
};

Decoded decode(uint32_t CIR) {
    return Decoded{
        .opcode = static_cast<uint8_t>(CIR & 0x7F),          // bits 0–6
        .funct3 = static_cast<uint8_t>((CIR >> 12) & 0x7),   // bits 12–14
        .funct7 = static_cast<uint8_t>((CIR >> 25) & 0x7F),  // bits 25–31
        .rd = static_cast<uint8_t>((CIR >> 7) & 0x1F),   // bits 7–11
        .rs1 = static_cast<uint8_t>((CIR >> 15) & 0x1F),  // bits 15–19
        .rs2 = static_cast<uint8_t>((CIR >> 20) & 0x1F),   // bits 20–24
        .imm = static_cast<uint16_t>((CIR >> 20) & 0xFFF) //as ram is only 8 bits 
    };
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