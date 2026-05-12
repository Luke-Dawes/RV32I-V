#include "Memory.h"
#include "../Instructions/R-type/R-Instructions.h"
#include "../Instructions/I-type/I-Instructions.h"
#include "../Instructions/S-type/S-Instructions.h"

uint8_t RAM[0x100];

//constexpr auto out_of_bounds = 0x1;
//constexpr auto miss_aligned_trap = 0x2;

Decoded_instruction decode(uint32_t PC) {
    Decoded_instruction d;

    
    d.opcode = PC & 0x7F;
    d.rd = (PC >> 7) & 0x1F;
    d.funct3 = (PC >> 12) & 0x7;
    d.rs1 = (PC >> 15) & 0x1F;
    d.rs2 = (PC >> 20) & 0x1F;
    d.funct7 = (PC >> 25) & 0x7F;

    switch (d.opcode)
    {
        // ---------------- R-type ----------------
    case 0x33:
        d.type = Instruction_type::R;
        d.imm = 0;
        break;

        // ---------------- I-type ----------------
    case 0x03: // loads
    case 0x13: // immediate ALU
    case 0x67: // JALR
    {
        d.type = Instruction_type::I;

        d.imm = (int32_t)PC >> 20; // sign-extend 12-bit
        break;
    }

    // ---------------- S-type (store) ----------------
    case 0x23:
    {
        d.type = Instruction_type::S;

        int32_t imm =
            ((PC >> 7) & 0x1F) |
            ((PC >> 25) & 0x7F) << 5;

        // sign-extend 12-bit
        if (imm & 0x800)
            imm |= 0xFFFFF000;

        d.imm = imm;
        break;
    }

    // ---------------- B-type (branch) ----------------
    case 0x63:
    {
        d.type = Instruction_type::B;

        int32_t imm =
            ((PC >> 8) & 0x0F) << 1 |  // bits 4:1
            ((PC >> 25) & 0x3F) << 5 |  // bits 10:5
            ((PC >> 7) & 0x01) << 11 |  // bit 11
            ((PC >> 31) & 0x01) << 12;   // bit 12 (sign)

        // sign-extend 13-bit
        if (imm & 0x1000)
            imm |= 0xFFFFE000;

        d.imm = imm;
        break;
    }

    // ---------------- U-type ----------------
    case 0x37: // LUI
    case 0x17: // AUIPC
    {
        d.type = Instruction_type::U;
        d.imm = (int32_t)(PC & 0xFFFFF000);
        break;
    }

    // ---------------- J-type ----------------
    case 0x6F:
    {
        d.type = Instruction_type::J;

        int32_t imm =
            ((PC >> 21) & 0x3FF) << 1 |  // bits 10:1
            ((PC >> 20) & 0x1) << 11 |  // bit 11
            ((PC >> 12) & 0xFF) << 12 |  // bits 19:12
            ((PC >> 31) & 0x1) << 20;   // sign bit

        // sign-extend 21-bit
        if (imm & 0x100000)
            imm |= 0xFFE00000;

        d.imm = imm;
        break;
    }

    default:
        d.type = Instruction_type::R;
        d.imm = 0;
        break;
    }

    return d;
}




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
    //Instructions[IDX(0, 0, 0x63)] = beq;
    //Instructions[IDX(0, 1, 0x63)] = bne;
    //Instructions[IDX(0, 4, 0x63)] = blt;
    //Instructions[IDX(0, 5, 0x63)] = bge;
    //Instructions[IDX(0, 6, 0x63)] = bltu;
    //Instructions[IDX(0, 7, 0x63)] = bgeu;

    // --- JUMP & UPPER IMM (Various Opcodes) ---
    //Instructions[IDX(0, 0, 0x6F)] = jal;   // Opcode 0x6F
    //Instructions[IDX(0, 0, 0x67)] = jalr;  // Opcode 0x67 (funct3=0)
    //Instructions[IDX(0, 0, 0x37)] = lui;   // Opcode 0x37
    //Instructions[IDX(0, 0, 0x17)] = auipc; // Opcode 0x17

    // --- SYSTEM (Opcode 0x73) ---
    //Instructions[IDX(0, 0, 0x73)] = system_call; // Handles ECALL/EBREAK
}

void init_RAM() { //needs to move
    for (auto i : RAM) {
        i = 0;
    }
}