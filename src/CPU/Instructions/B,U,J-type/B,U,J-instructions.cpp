#include "B,J,U-instructions.h"
#include "../../CPU.h"
#include "../../../Memory/Memory.h"
#include <iostream>


// ----- B type ----------
void beq(CPU& cpu, Decoded_instruction& ins) {
	if (cpu.registers[ins.rs1] == cpu.registers[ins.rs2]) {
		cpu.PC += ins.imm;
		cpu.branch_happended = true;
	}

}

void bne(CPU& cpu, Decoded_instruction& ins) {
	if (cpu.registers[ins.rs1] != cpu.registers[ins.rs2]) {
		cpu.PC += ins.imm;
		cpu.branch_happended = true;
	}

}

void blt(CPU& cpu, Decoded_instruction& ins) {
	if (static_cast<int32_t>(cpu.registers[ins.rs1]) < static_cast<int32_t>(cpu.registers[ins.rs2])) {
		cpu.PC += ins.imm;
		cpu.branch_happended = true;
	}
}

void bge(CPU& cpu, Decoded_instruction& ins) {
	if (static_cast<int32_t>(cpu.registers[ins.rs1]) >= static_cast<int32_t>(cpu.registers[ins.rs2])) {
		cpu.PC += ins.imm;
		cpu.branch_happended = true;
	}
}

void bltu(CPU& cpu, Decoded_instruction& ins) { //zero extends
	if (cpu.registers[ins.rs1] < cpu.registers[ins.rs2]) {
		cpu.PC += ins.imm;
		cpu.branch_happended = true;
	}
}

void bgeu(CPU& cpu, Decoded_instruction& ins) { //zero extends - imm is a int but the rest is uint
	if (cpu.registers[ins.rs1] >= cpu.registers[ins.rs2]) {
		cpu.PC += ins.imm;
		cpu.branch_happended = true;
	}
}

// ----- J type ----------
void jal(CPU& cpu, Decoded_instruction& ins) {
	if (ins.rd != 0) {
		cpu.registers[ins.rd] = cpu.PC + 4;
	}

		cpu.PC += ins.imm;
		std::cout << "JAL happended and PC is currently =" << cpu.PC << "\n";
		cpu.branch_happended = true;
	

}

// ----- U type ----------
void lui(CPU& cpu, Decoded_instruction& ins) {
	cpu.registers[ins.rd] = ins.imm;
}

void auipc(CPU& cpu, Decoded_instruction& ins) {
	cpu.registers[ins.rd] = cpu.PC + ins.imm;
}