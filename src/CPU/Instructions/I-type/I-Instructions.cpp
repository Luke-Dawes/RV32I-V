#include "I-Instructions.h"
#include "../../CPU.h"
#include "../../../Memory/Memory.h"

/*inline int32_t sixteen_to_thirtytwo(uint16_t ins.imm) {
	return (static_cast<int32_t>(ins.imm) << 20) >> 20;
}
*/

void addi(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] + ins.imm;
}

void xori(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] ^ ins.imm;
}

void ori(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] | ins.imm;
}

void andi(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] & ins.imm;
}

void slli(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] << (ins.imm & 0x1F);
}

void srli(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] >> (ins.imm & 0x1F);
}

void srai(CPU& cpu, Decoded_instruction& ins) { //msb extends - signed
	if (ins.rd != 0) cpu.registers[ins.rd] = static_cast<int32_t>(cpu.registers[ins.rs1]) >> (ins.imm & 0x1F);
}

void slti(CPU& cpu, Decoded_instruction& ins) { //signed
	if (ins.rd != 0) cpu.registers[ins.rd] = (static_cast<int32_t>(cpu.registers[ins.rs1]) < ins.imm) ? 1 : 0;
}

void sltiu(CPU& cpu, Decoded_instruction& ins) { //unsigned
	if (ins.rd != 0) cpu.registers[ins.rd] = (cpu.registers[ins.rs1] < static_cast<uint32_t>(ins.imm)) ? 1 : 0;
}


void lb(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) {
		cpu.registers[ins.rd] = static_cast<int32_t>(static_cast<int8_t>(cpu.memory.read8(cpu.registers[ins.rs1] + ins.imm)));
	}
}

//signed
void lh(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) {
		int32_t val = cpu.memory.read16(cpu.registers[ins.rs1] + ins.imm);
		cpu.registers[ins.rd] = (val << 16) >> 16;
	}
}
//signed
void lw(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) {
		cpu.registers[ins.rd] = static_cast<int32_t>(cpu.memory.read32(cpu.registers[ins.rs1] + ins.imm));
	}
}
//unsigned
void lbu(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) {
		cpu.registers[ins.rd] = static_cast<uint32_t>(cpu.memory.read8(cpu.registers[ins.rs1] + ins.imm));
	}
}

void lhu(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) {
		cpu.registers[ins.rd] = static_cast<uint32_t>(cpu.memory.read16(cpu.registers[ins.rs1] + ins.imm));
	}
}


void jalr(CPU& cpu, Decoded_instruction& ins) {
	cpu.registers[ins.rd] = cpu.PC + 4;
	cpu.PC = cpu.registers[ins.rs1] + ins.imm;
}

void ecall(CPU& cpu, Decoded_instruction& ins) {}
void ebreak(CPU& cpu, Decoded_instruction& ins) {}