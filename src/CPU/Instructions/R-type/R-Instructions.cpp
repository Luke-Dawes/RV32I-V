#include "R-Instructions.h"
#include "../../CPU.h"
#include "../../../Memory/Memory.h"
#include "../../Trap.h"

std::optional<Trap> add(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] + cpu.registers[ins.rs2];
	return std::nullopt;
}

std::optional<Trap> sub(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] - cpu.registers[ins.rs2];
	return std::nullopt;
}

std::optional<Trap> _xor(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] ^ cpu.registers[ins.rs2];
	return std::nullopt;
}

std::optional<Trap> _or(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] | cpu.registers[ins.rs2];
	return std::nullopt;
}

std::optional<Trap> _and(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] & cpu.registers[ins.rs2];
	return std::nullopt;
}

std::optional<Trap> sll(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] << (cpu.registers[ins.rs2] & 0x1F);
	return std::nullopt;
}

std::optional<Trap> srl(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] >> (cpu.registers[ins.rs2] & 0x1F);
	return std::nullopt;
}

//if the number is negative it the empty slots created by shifting are filled with 1s.
std::optional<Trap> sra(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = static_cast<int32_t>(cpu.registers[ins.rs1]) >> (cpu.registers[ins.rs2] & 0x1F);
	return std::nullopt;
}

//signed comparison
std::optional<Trap> slt(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = (static_cast<int32_t>(cpu.registers[ins.rs1]) < static_cast<int32_t>(cpu.registers[ins.rs2])) ? 1 : 0;
	return std::nullopt;
}

//unsigned comparision
std::optional<Trap> sltu(CPU& cpu, const Decoded_instruction& ins) { //theres a note of zero-extends - no sign 
	if (ins.rd != 0) cpu.registers[ins.rd] = (cpu.registers[ins.rs1] < cpu.registers[ins.rs2]) ? 1 : 0;
	return std::nullopt;
}


//m extension 

std::optional<Trap> mul(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = static_cast<uint32_t>((uint64_t)cpu.registers[ins.rs1] * (uint64_t)cpu.registers[ins.rs2]);
	return std::nullopt;
}

std::optional<Trap> mulh(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) {
		int64_t a = (int64_t)(int32_t)cpu.registers[ins.rs1];
		int64_t b = (int64_t)(int32_t)cpu.registers[ins.rs2];

		cpu.registers[ins.rd] = (uint32_t)((a * b) >> 32);
	}
	return std::nullopt;
}

std::optional<Trap> mulhsu(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) {
		int64_t a = (int64_t)(int32_t)cpu.registers[ins.rs1];
		
		cpu.registers[ins.rd] = (uint32_t)((a * cpu.registers[ins.rs2]) >> 32);
	}
	return std::nullopt;
}


std::optional<Trap> mulhu(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = static_cast<uint32_t>(((uint64_t)cpu.registers[ins.rs1] * (uint64_t)cpu.registers[ins.rs2]) >> 32);
	return std::nullopt;
}

std::optional<Trap> div(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) {
		
		if (cpu.registers[ins.rs2] == 0) {
			cpu.registers[ins.rd] = 0xFFFFFFFFu; 
			return std::nullopt;
		}

		if ((int32_t)cpu.registers[ins.rs1] == INT32_MIN && (int32_t)cpu.registers[ins.rs2] == -1) {
			cpu.registers[ins.rd] = static_cast<uint32_t>(INT32_MIN); 
			return std::nullopt;
		}

		cpu.registers[ins.rd] = static_cast<uint32_t>((int32_t)cpu.registers[ins.rs1] / (int32_t)cpu.registers[ins.rs2]);
	}
	return std::nullopt;
}

std::optional<Trap> divu(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) {
		if (cpu.registers[ins.rs2] == 0) {
			cpu.registers[ins.rd] = 0xFFFFFFFFu; 
			return std::nullopt;
		}

		cpu.registers[ins.rd] = cpu.registers[ins.rs1] / cpu.registers[ins.rs2];
	}
	return std::nullopt;
}

std::optional<Trap> rem(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) {
		if (cpu.registers[ins.rs2] == 0) {
			cpu.registers[ins.rd] = cpu.registers[ins.rs1]; 
			return std::nullopt;
		}

		if ((int32_t)cpu.registers[ins.rs1] == INT32_MIN && (int32_t)cpu.registers[ins.rs2] == -1) {
			cpu.registers[ins.rd] = 0;
			return std::nullopt;
		}


		cpu.registers[ins.rd] = (int32_t)cpu.registers[ins.rs1] % (int32_t)cpu.registers[ins.rs2];
	}
	return std::nullopt;
}

std::optional<Trap> remu(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) {
		if (cpu.registers[ins.rs2] == 0) {
			cpu.registers[ins.rd] = cpu.registers[ins.rs1];
			return std::nullopt;
		}

		cpu.registers[ins.rd] = cpu.registers[ins.rs1] % cpu.registers[ins.rs2];
	}
	return std::nullopt;
}