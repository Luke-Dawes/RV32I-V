#include "S-Instructions.h"
#include "../../CPU.h"
#include "../../../Memory/Memory.h"


std::optional<Trap> sb(CPU& cpu, const Decoded_instruction& ins) {

	uint32_t address = cpu.registers[ins.rs1] + ins.imm;

	uint8_t value = cpu.registers[ins.rs2];

	return cpu.store8(address,value);
}

std::optional<Trap> sh(CPU& cpu, const Decoded_instruction& ins) {
	uint32_t addr = cpu.registers[ins.rs1] + ins.imm;

	uint16_t value = cpu.registers[ins.rs2];

	return cpu.store16(addr, value);
}
std::optional<Trap> sw(CPU& cpu, const Decoded_instruction& ins) {
	uint32_t addr = cpu.registers[ins.rs1] + ins.imm;

	int32_t value = cpu.registers[ins.rs2];

	return cpu.store32(addr, value);
}