#include "Memory.h"

uint8_t RAM[0x100];
uint32_t registers[31];
int32_t PC = 0;

//constexpr auto out_of_bounds = 0x1;
//constexpr auto miss_aligned_trap = 0x2;

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

void init_memory() {
    for (auto i : RAM) {
        i = 0;
    }
    for (auto i : registers) {
        i = 0;
    }
}