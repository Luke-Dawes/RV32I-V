#include "S-Instructions.h"
#include "../../CPU.h"
#include "../../../Memory/Memory.h"


void sb(CPU& cpu, Decoded_instruction& ins) {
	cpu.memory.write8(cpu.registers[ins.rs1] + ins.imm, cpu.registers[ins.rs2] & 0xFF);
}

void sh(CPU& cpu, Decoded_instruction& ins) {
	uint32_t addr = cpu.registers[ins.rs1] + ins.imm;

	uint16_t value = cpu.registers[ins.rs2];

	cpu.memory.write16(addr, value);

}
void sw(CPU& cpu, Decoded_instruction& ins) {
	uint32_t addr = cpu.registers[ins.rs1] + ins.imm;

	int32_t value = cpu.registers[ins.rs2];

	cpu.memory.write32(addr, value);

}