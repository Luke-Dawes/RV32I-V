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


std::optional<Trap> lb(CPU& cpu, const Decoded_instruction& ins) { 
	uint32_t value;

	uint32_t address = cpu.registers[ins.rs1] + ins.imm;

	if (auto trap = cpu.load32(address, value))
		return trap;

	if (ins.rd != 0)
		cpu.registers[ins.rd] = value;

	return std::nullopt;
}

//signed
std::optional<Trap> lh(CPU& cpu, const Decoded_instruction& ins) {
	uint16_t value;

	uint32_t address = cpu.registers[ins.rs1] + ins.imm;

	if (auto trap = cpu.load16(address, value))
		return trap;

	if (ins.rd != 0)
		cpu.registers[ins.rd] = static_cast<int32_t>(static_cast<int16_t>(value));

	return std::nullopt;
}
//signed
std::optional<Trap> lw(CPU& cpu, const Decoded_instruction& ins) {
	uint32_t value;

	uint32_t address = cpu.registers[ins.rs1] + ins.imm;

	if (auto trap = cpu.load32(address, value))
		return trap;

	if (ins.rd != 0)
		cpu.registers[ins.rd] = value;

	return std::nullopt;
}
//unsigned
std::optional<Trap> lbu(CPU& cpu, const Decoded_instruction& ins) {
	uint8_t value;

	uint32_t address = cpu.registers[ins.rs1] + ins.imm;

	if (auto trap = cpu.load8(address, value))
		return trap;

	if (ins.rd != 0)
		cpu.registers[ins.rd] = value;

	return std::nullopt;
}

std::optional<Trap> lhu(CPU& cpu, const Decoded_instruction& ins) {
	uint16_t value;

	uint32_t address = cpu.registers[ins.rs1] + ins.imm;

	if (auto trap = cpu.load16(address, value))
		return trap;

	if (ins.rd != 0)
		cpu.registers[ins.rd] = value;

	return std::nullopt;
}


std::optional<Trap> jalr(CPU& cpu, const Decoded_instruction& ins) {
	
	uint32_t target = cpu.registers[ins.rs1] + ins.imm;

	target &= ~1;

	if (target & 3)
		return Trap::InstructionAddressMisaligned(target);

	if (ins.rd != 0)
		cpu.registers[ins.rd] = cpu.PC + 4;

	cpu.PC = target;

	return std::nullopt;
}

std::optional<Trap> system(CPU& cpu, const Decoded_instruction& ins) {
	switch (ins.imm)
	{
	case 0x000: // ECALL
		return Trap::EcallM();

	case 0x001: // EBREAK
		return Trap::Breakpoint();
		

	case 0x302: // MRET
		cpu.PC = cpu.csrs.read(CSR_LOCATIONS::MEPC);
		return std::nullopt;


	case 0x105: // WFI
		// Can be a no-op for now.
		return std::nullopt;

	default:
		return Trap::IllegalInstruction(ins.full);
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
