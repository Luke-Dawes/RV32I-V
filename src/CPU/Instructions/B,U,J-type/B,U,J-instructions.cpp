#include "B,J,U-instructions.h"
#include "../../CPU.h"
#include "../../../Memory/Memory.h"
#include "../../Trap.h"
#include <iostream>


// ----- B type ----------
std::optional<Trap> beq(CPU& cpu, const Decoded_instruction& ins) {
	if (cpu.registers[ins.rs1] == cpu.registers[ins.rs2]) {
		cpu.PC += ins.imm;
		cpu.branch_happended = true;
	}
	return std::nullopt;
}

std::optional<Trap> bne(CPU& cpu, const Decoded_instruction& ins) {
	if (cpu.registers[ins.rs1] != cpu.registers[ins.rs2]) {
		cpu.PC += ins.imm;
		cpu.branch_happended = true;
	}
	return std::nullopt;
}

std::optional<Trap> blt(CPU& cpu, const Decoded_instruction& ins) {
	if (static_cast<int32_t>(cpu.registers[ins.rs1]) < static_cast<int32_t>(cpu.registers[ins.rs2])) {
		cpu.PC += ins.imm;
		cpu.branch_happended = true;
	}
	return std::nullopt;
}

std::optional<Trap> bge(CPU& cpu, const Decoded_instruction& ins) {
	if (static_cast<int32_t>(cpu.registers[ins.rs1]) >= static_cast<int32_t>(cpu.registers[ins.rs2])) {
		cpu.PC += ins.imm;
		cpu.branch_happended = true;
	}
	return std::nullopt;
}

std::optional<Trap> bltu(CPU& cpu, const Decoded_instruction& ins) { //zero extends
	if (cpu.registers[ins.rs1] < cpu.registers[ins.rs2]) {
		cpu.PC += ins.imm;
		cpu.branch_happended = true;
	}
	return std::nullopt;
}

std::optional<Trap> bgeu(CPU& cpu, const Decoded_instruction& ins) { //zero extends - imm is a int but the rest is uint
	if (cpu.registers[ins.rs1] >= cpu.registers[ins.rs2]) {
		cpu.PC += ins.imm;
		cpu.branch_happended = true;
	}
	return std::nullopt;
}

// ----- J type ----------
std::optional<Trap> jal(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) {
		cpu.registers[ins.rd] = cpu.PC + 4;
	}

	cpu.PC += ins.imm;
	std::cout << "JAL happended and PC is currently =" << cpu.PC << "\n";
	cpu.branch_happended = true;

	return std::nullopt;
}

// ----- U type ----------
std::optional<Trap> lui(CPU& cpu, const Decoded_instruction& ins) {
	cpu.registers[ins.rd] = ins.imm;
	return std::nullopt;
}

std::optional<Trap> auipc(CPU& cpu, const Decoded_instruction& ins) {
	cpu.registers[ins.rd] = cpu.PC + ins.imm;
	return std::nullopt;
}