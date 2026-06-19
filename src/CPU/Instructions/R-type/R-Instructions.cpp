#include "R-Instructions.h"
#include "../../CPU.h"
#include "../../../Memory/Memory.h"

void add(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] + cpu.registers[ins.rs2];
}

void sub(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] - cpu.registers[ins.rs2];
}

void _xor(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] ^ cpu.registers[ins.rs2];
}

void _or(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] | cpu.registers[ins.rs2];
}

void _and(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] & cpu.registers[ins.rs2];
}

void sll(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] << (cpu.registers[ins.rs2] & 0x1F);
}

void srl(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] >> (cpu.registers[ins.rs2] & 0x1F);
}

//if the number is negative it the empty slots created by shifting are filled with 1s.
void sra(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = static_cast<int32_t>(cpu.registers[ins.rs1]) >> (cpu.registers[ins.rs2] & 0x1F);
}

//signed comparison
void slt(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = (static_cast<int32_t>(cpu.registers[ins.rs1]) < static_cast<int32_t>(cpu.registers[ins.rs2])) ? 1 : 0;
}

//unsigned comparision
void sltu(CPU& cpu, Decoded_instruction& ins) { //theres a note of zero-extends - no sign 
	if (ins.rd != 0) cpu.registers[ins.rd] = (cpu.registers[ins.rs1] < cpu.registers[ins.rs2]) ? 1 : 0;
}


//m extension 

void mul(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = static_cast<uint32_t>((uint64_t)cpu.registers[ins.rs1] * (uint64_t)cpu.registers[ins.rs2]);
}

void mulh(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) {
		int64_t a = (int64_t)(int32_t)cpu.registers[ins.rs1];
		int64_t b = (int64_t)(int32_t)cpu.registers[ins.rs2];

		cpu.registers[ins.rd] = (uint32_t)((a * b) >> 32);
	}
}

void mulhsu(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) {
		int64_t a = (int64_t)(int32_t)cpu.registers[ins.rs1];
		
		cpu.registers[ins.rd] = (uint32_t)((a * cpu.registers[ins.rs2]) >> 32);
	}
}


void mulhu(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = static_cast<uint32_t>(((uint64_t)cpu.registers[ins.rs1] * (uint64_t)cpu.registers[ins.rs2]) >> 32);
}

void div(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) {
		
		if (cpu.registers[ins.rs2] == 0) {
			cpu.registers[ins.rd] = 0xFFFFFFFFu; 
			return;
		}

		if ((int32_t)cpu.registers[ins.rs1] == INT32_MIN && (int32_t)cpu.registers[ins.rs2] == -1) {
			cpu.registers[ins.rd] = static_cast<uint32_t>(INT32_MIN); 
			return;
		}

		cpu.registers[ins.rd] = static_cast<uint32_t>((int32_t)cpu.registers[ins.rs1] / (int32_t)cpu.registers[ins.rs2]);
	}
}

void divu(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) {
		if (cpu.registers[ins.rs2] == 0) {
			cpu.registers[ins.rd] = 0xFFFFFFFFu; 
			return;
		}

		cpu.registers[ins.rd] = cpu.registers[ins.rs1] / cpu.registers[ins.rs2];
	}
}

void rem(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) {
		if (cpu.registers[ins.rs2] == 0) {
			cpu.registers[ins.rd] = cpu.registers[ins.rs1]; 
			return;
		}

		if ((int32_t)cpu.registers[ins.rs1] == INT32_MIN && (int32_t)cpu.registers[ins.rs2] == -1) {
			cpu.registers[ins.rd] = 0;
			return;
		}


		cpu.registers[ins.rd] = (int32_t)cpu.registers[ins.rs1] % (int32_t)cpu.registers[ins.rs2];
	}
}

void remu(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) {
		if (cpu.registers[ins.rs2] == 0) {
			cpu.registers[ins.rd] = cpu.registers[ins.rs1];
			return;
		}

		cpu.registers[ins.rd] = cpu.registers[ins.rs1] % cpu.registers[ins.rs2];
	}
}