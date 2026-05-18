#include "B,J,U-instructions.h"
#include "../../CPU/CPU.h"
#include "../../CPU/Memory.h"


// ----- B type ----------
void beq(CPU& cpu, Decoded_instruction& ins) {
	if (cpu.registers[ins.rs1] == cpu.registers[ins.rs2]) cpu.PC += ins.imm;
}

void bne(CPU& cpu, Decoded_instruction& ins) {
	if (cpu.registers[ins.rs1] != cpu.registers[ins.rs2]) cpu.PC += ins.imm;
}

void blt(CPU& cpu, Decoded_instruction& ins) {
	if (static_cast<int32_t>(cpu.registers[ins.rs1]) < static_cast<int32_t>(cpu.registers[ins.rs2])) cpu.PC += ins.imm;
}

void bge(CPU& cpu, Decoded_instruction& ins) {
	if (static_cast<int32_t>(cpu.registers[ins.rs1]) >= static_cast<int32_t>(cpu.registers[ins.rs2])) cpu.PC += ins.imm;
}

void bltu(CPU& cpu, Decoded_instruction& ins) { //zero extends
	if (cpu.registers[ins.rs1] < cpu.registers[ins.rs2]) cpu.PC += ins.imm;
}

void bgeu(CPU& cpu, Decoded_instruction& ins) { //zero extends - imm is a int but the rest is uint
	if (cpu.registers[ins.rs1] >= cpu.registers[ins.rs2]) cpu.PC += ins.imm;
}

// ----- J type ----------
void jal(CPU& cpu, Decoded_instruction& ins) {
	cpu.registers[ins.rd] = cpu.PC + 4; 
	cpu.PC += ins.imm;
}

// ----- U type ----------
void lui(CPU& cpu, Decoded_instruction& ins) {
	cpu.registers[ins.rd] = ins.imm << 12;
}

void auipc(CPU& cpu, Decoded_instruction& ins) {
	cpu.registers[ins.rd] = cpu.PC + (ins.imm << 12);
}