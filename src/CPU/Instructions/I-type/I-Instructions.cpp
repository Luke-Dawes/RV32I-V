#include "I-Instructions.h"
#include "../../CPU.h"
#include "../../../Memory/Memory.h"
#include "../../CSR.h"

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

void system(CPU& cpu, Decoded_instruction& ins) {
	switch (ins.imm)
	{
	case 0x000: // ECALL
		cpu.raise_trap(11, 0);
		break;

	case 0x001: // EBREAK
		cpu.raise_trap(3, 0);
		break;

	case 0x302: // MRET
		cpu.PC = cpu.csrs.read(CSR_LOCATIONS::MEPC);
		break;

	case 0x105: // WFI
		// Can be a no-op for now.
		break;

	default:
		cpu.raise_trap(2, ins.full); // Illegal instruction
		break;
	}
}

void csrrw(CPU& cpu, Decoded_instruction& ins) {

	uint32_t old = cpu.csrs.read(ins.csr);

	if (ins.rd != 0) {
		cpu.registers[ins.rd] = old;
	}

	cpu.csrs.write(ins.csr, cpu.registers[ins.rs1]);
}

void csrrs(CPU& cpu, Decoded_instruction& ins) {

	uint32_t old = cpu.csrs.read(ins.csr);

	if (ins.rd != 0)
		cpu.registers[ins.rd] = old;

	if (ins.rs1 != 0)
		cpu.csrs.write(ins.csr, old | cpu.registers[ins.rs1]);

}

void csrrc(CPU& cpu, Decoded_instruction& ins) {
	uint32_t old = cpu.csrs.read(ins.csr);

	if (ins.rd != 0)
		cpu.registers[ins.rd] = old;

	if (ins.rs1 != 0)
		cpu.csrs.write(ins.csr, old & ~cpu.registers[ins.rs1]);
}

void csrrwi(CPU& cpu, Decoded_instruction& ins) {
	uint32_t old = cpu.csrs.read(ins.csr);

	if (ins.rd != 0)
		cpu.registers[ins.rd] = old;

	cpu.csrs.write(ins.csr, ins.imm & 0x1F);
}

void csrrsi(CPU& cpu, Decoded_instruction& ins) {

	uint32_t old = cpu.csrs.read(ins.csr);

	if (ins.rd != 0) 
		cpu.registers[ins.rd] = old;
	
	uint32_t new_imm = ins.imm & 0x1F;

	if (new_imm != 0)
		cpu.csrs.write(ins.csr, old | new_imm);

}
void csrrci(CPU& cpu, Decoded_instruction& ins) {

	uint32_t old = cpu.csrs.read(ins.csr);

	if (ins.rd != 0)
		cpu.registers[ins.rd] = old;

	uint32_t new_imm = ins.imm & 0x1F;

	if (new_imm != 0)
		cpu.csrs.write(ins.csr, old & ~new_imm);
}
