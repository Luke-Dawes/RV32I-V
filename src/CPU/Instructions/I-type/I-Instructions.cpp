#include "I-Instructions.h"
#include "../../CPU.h"
#include "../../../Memory/Memory.h"
#include "../../CSR.h"
#include "../../Trap.h"
#include <iostream>



std::optional<Trap> addi(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] + ins.imm;
	return std::nullopt;
}

std::optional<Trap> xori(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] ^ ins.imm;
	return std::nullopt;
}

std::optional<Trap> ori(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] | ins.imm;
	return std::nullopt;
}

std::optional<Trap> andi(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] & ins.imm;
	return std::nullopt;
}

std::optional<Trap> slli(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] << (ins.imm & 0x1F);
	return std::nullopt;
}

std::optional<Trap> srli(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) cpu.registers[ins.rd] = cpu.registers[ins.rs1] >> (ins.imm & 0x1F);
	return std::nullopt;
}

std::optional<Trap> srai(CPU& cpu, const Decoded_instruction& ins) { //msb extends - signed
	if (ins.rd != 0) cpu.registers[ins.rd] = static_cast<int32_t>(cpu.registers[ins.rs1]) >> (ins.imm & 0x1F);
	return std::nullopt;
}

std::optional<Trap> slti(CPU& cpu, const Decoded_instruction& ins) { //signed
	if (ins.rd != 0) cpu.registers[ins.rd] = (static_cast<int32_t>(cpu.registers[ins.rs1]) < ins.imm) ? 1 : 0;
	return std::nullopt;
}

std::optional<Trap> sltiu(CPU& cpu, const Decoded_instruction& ins) { //unsigned
	if (ins.rd != 0) cpu.registers[ins.rd] = (cpu.registers[ins.rs1] < static_cast<uint32_t>(ins.imm)) ? 1 : 0;
	return std::nullopt;
}


std::optional<Trap> lb(CPU& cpu, const Decoded_instruction& ins) { this needs to change
	if (ins.rd != 0) {
		cpu.registers[ins.rd] = static_cast<int32_t>(static_cast<int8_t>(cpu.memory.read8(cpu.registers[ins.rs1] + ins.imm))); #========================================================================================================
	}
}

//signed
std::optional<Trap> lh(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) {
		int32_t val = cpu.memory.read16(cpu.registers[ins.rs1] + ins.imm); ###################
		cpu.registers[ins.rd] = (val << 16) >> 16;
	}
}
//signed
std::optional<Trap> lw(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) {
		cpu.registers[ins.rd] = static_cast<int32_t>(cpu.memory.read32(cpu.registers[ins.rs1] + ins.imm)); ###############
	}
}
//unsigned
std::optional<Trap> lbu(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) {
		cpu.registers[ins.rd] = static_cast<uint32_t>(cpu.memory.read8(cpu.registers[ins.rs1] + ins.imm)); ##########
	}
}

std::optional<Trap> lhu(CPU& cpu, const Decoded_instruction& ins) {
	if (ins.rd != 0) {
		cpu.registers[ins.rd] = static_cast<uint32_t>(cpu.memory.read16(cpu.registers[ins.rs1] + ins.imm)); #############
	}
}


std::optional<Trap> jalr(CPU& cpu, const Decoded_instruction& ins) {
	cpu.registers[ins.rd] = cpu.PC + 4;
	cpu.PC = cpu.registers[ins.rs1] + ins.imm;
	return std::nullopt;
}

std::optional<Trap> system(CPU& cpu, const Decoded_instruction& ins) { ##################
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

std::optional<Trap> csrrw(CPU& cpu, const Decoded_instruction& ins) {

	uint32_t old = cpu.csrs.read(ins.csr);

	if (ins.rd != 0) {
		cpu.registers[ins.rd] = old;
	}

	cpu.csrs.write(ins.csr, cpu.registers[ins.rs1]);
	return std::nullopt;
}

std::optional<Trap> csrrs(CPU& cpu, const Decoded_instruction& ins) {

	uint32_t old = cpu.csrs.read(ins.csr);

	if (ins.rd != 0)
		cpu.registers[ins.rd] = old;

	if (ins.rs1 != 0)
		cpu.csrs.write(ins.csr, old | cpu.registers[ins.rs1]);
	return std::nullopt;

}

std::optional<Trap> csrrc(CPU& cpu, const Decoded_instruction& ins) {
	uint32_t old = cpu.csrs.read(ins.csr);

	if (ins.rd != 0)
		cpu.registers[ins.rd] = old;

	if (ins.rs1 != 0)
		cpu.csrs.write(ins.csr, old & ~cpu.registers[ins.rs1]);
	return std::nullopt;
}

std::optional<Trap> csrrwi(CPU& cpu, const Decoded_instruction& ins) {
	uint32_t old = cpu.csrs.read(ins.csr);

	if (ins.rd != 0)
		cpu.registers[ins.rd] = old;

	cpu.csrs.write(ins.csr, ins.imm & 0x1F);
	return std::nullopt;
}

std::optional<Trap> csrrsi(CPU& cpu, const Decoded_instruction& ins) {

	uint32_t old = cpu.csrs.read(ins.csr);

	if (ins.rd != 0) 
		cpu.registers[ins.rd] = old;
	
	uint32_t new_imm = ins.imm & 0x1F;

	if (new_imm != 0)
		cpu.csrs.write(ins.csr, old | new_imm);
	return std::nullopt;

}
std::optional<Trap> csrrci(CPU& cpu, const Decoded_instruction& ins) {

	uint32_t old = cpu.csrs.read(ins.csr);

	if (ins.rd != 0)
		cpu.registers[ins.rd] = old;

	uint32_t new_imm = ins.imm & 0x1F;

	if (new_imm != 0)
		cpu.csrs.write(ins.csr, old & ~new_imm);
	return std::nullopt;
}
