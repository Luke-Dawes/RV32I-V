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