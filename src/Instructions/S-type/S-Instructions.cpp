#include "S-Instructions.h"


void sb(CPU& cpu, Decoded_instruction& ins) {
	RAM[cpu.registers[ins.rs1] + ins.imm] = cpu.registers[ins.rs2] & 0xFF;
}

void sh(CPU& cpu, Decoded_instruction& ins) {
	uint32_t addr = cpu.registers[ins.rs1] + ins.imm;

	uint16_t value = cpu.registers[ins.rs2];

	RAM[addr] = value & 0xFF;
	RAM[addr + 1] = (value >> 8) & 0xFF;

}
void sw(CPU& cpu, Decoded_instruction& ins) {
	uint32_t addr = cpu.registers[ins.rs1] + ins.imm;

	int32_t value = cpu.registers[ins.rs2];

	RAM[addr] = value & 0xFF;
	RAM[addr + 1] = (value >> 8) & 0xFF;
	RAM[addr + 2] = (value >> 16) & 0xFF;
	RAM[addr + 3] = (value >> 24) & 0xFF;
}