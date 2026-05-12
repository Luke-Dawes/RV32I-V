#include "S-Instructions.h"


void sb(CPU& cpu, Decoded_instruction& ins) {
	RAM[cpu.registers[ins.rs1] + ins.imm] = cpu.registers[ins.rs2] & 0xFF;
}

void sh(CPU& cpu, Decoded_instruction& ins) {
	RAM[cpu.registers[ins.rs1] + ins.imm] = cpu.registers[ins.rs2] & 0xFFFF;
}
void sw(CPU& cpu, Decoded_instruction& ins) {
	RAM[cpu.registers[ins.rs1] + ins.imm] = cpu.registers[ins.rs2];
}