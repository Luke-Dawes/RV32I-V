#pragma once
#include <cstdint>

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

inline uint8_t get_opcode(uint32_t CIR) {
	return (uint8_t)CIR & 0x7F;
}

uint8_t get_funct3(uint32_t CIR) {
    //14-12 bits
    uint8_t temp =
        (CIR >> 12) & 0x7; 

}

void decode_instruction(uint32_t CIR) {
    // Extract the major opcode (bits 0-6)
    uint8_t opcode = CIR & 0x7F;
    //extract the funct3 which is in the 12-14 bits
    uint8_t funct3 = (CIR >> 12) & 0x7;

    switch (opcode) {
        case 0x33:

            switch (funct3) {

                case 0x0:
                    //need funct7 to tell - either add or sub
                    break;
                case 0x1:
                    //logical left shift 
                    break;
                case 0x2:
                    //set less than
                    break;
                case 0x3:
                    //set less than (u)
                    break;
                case 0x4:
                    //xor
                    break;
                case 0x5:
                    //shift right logical
                    //shift right arith
                    break;
                case 0x6:
                    //or
                    break;
                case 0x7:
                    //and
                    break;
                default:
                    //unknown
                    break;
            }

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