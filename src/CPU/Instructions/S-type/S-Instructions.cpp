#include "S-Instructions.h"
#include "../../CPU.h"
#include "../../../Memory/Memory.h"


std::optional<Trap> sb(CPU& cpu, const Decoded_instruction& ins) {
	cpu.memory.write8(cpu.registers[ins.rs1] + ins.imm, cpu.registers[ins.rs2] & 0xFF); ##############################
	return std::nullopt;
}

std::optional<Trap> sh(CPU& cpu, const Decoded_instruction& ins) {
	uint32_t addr = cpu.registers[ins.rs1] + ins.imm;

	uint16_t value = cpu.registers[ins.rs2];

	cpu.memory.write16(addr, value); ##################################
	return std::nullopt;
}
std::optional<Trap> sw(CPU& cpu, const Decoded_instruction& ins) {
	uint32_t addr = cpu.registers[ins.rs1] + ins.imm;

	int32_t value = cpu.registers[ins.rs2];

	cpu.memory.write32(addr, value); #################
	return std::nullopt;
}