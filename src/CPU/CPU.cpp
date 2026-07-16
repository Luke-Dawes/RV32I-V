#include "CPU.h"
#include "../Memory/Memory.h"
#include "CSR.h"
#include <iostream>
#include "Trap.h"

CPU::CPU(Memory& mem) : memory(mem) {}

fetched_result CPU::fetch() {

	fetched_result result; 

	if (PC < memory.RAM_BASE || PC - memory.RAM_BASE > memory.memory_size - 4) {
		//throw error
		result.trap = Trap::InstructionAccessFault(PC);
		return result;
	}

	if (PC % 4 != 0) {
		result.trap = Trap::InstructionAddressMisaligned(PC);
		return result;
	}
	
	uint32_t value = 0;
	MemoryError error = memory.read32(PC, value);

	switch (error) 
	{
	case MemoryError::None:
		result.instruction = value;
		break;

	case MemoryError::Misaligned:
		result.trap = Trap::InstructionAddressMisaligned(PC);
		break;

	case MemoryError::AccessFault:
		result.trap = Trap::InstructionAccessFault(PC);
		break;
	
	}

		
	return result;
}

Decoded_instruction CPU::decode(uint32_t ins) {
	return decode_ins(ins);
}


std::optional<Trap> CPU::execute(Decoded_instruction d) {

	std::cout << "PC = " << std::hex << PC << std::endl;

	uint32_t key = make_key(d.funct7, d.funct3, d.opcode);

	if (Instructions[key] != nullptr) {
		auto it = instruction_debug_table.find(key);

		if (it != instruction_debug_table.end()) {
			std::cout << "instruction is " << it->second << "\n";
		}
		else
		{
			std::cout << "instruction found in instruction table but not map";
		}

		return Instructions[key](*this, d);
	}
	else {
		std::cout << "unknown instruction, key=" << key << "\n";
		return Trap::IllegalInstruction(d.full);
	}
}

void CPU::tick() { 
	branch_happended = false;


	auto fetched = fetch();

	if (fetched.trap) 
	{
		enter_trap(*fetched.trap);
		return;
	}

	std::cout << "RAW INSTRUCTION: 0x" << std::hex << fetched.instruction << "\n";

	Decoded_instruction decoded = decode(fetched.instruction);

	if (auto trap = execute(decoded)) {
		enter_trap(*trap);
	}

	if (!branch_happended) {
		PC += 4;
	}
}

void CPU::enter_trap(const Trap& trap) {

	std::cout << "\n\n raising trap \n\n";

	csrs.write(CSR_LOCATIONS::MEPC, PC);

	uint32_t cause = trap.cause;

	if (trap.interrupt)
		cause |= (1u << 31);

	csrs.write(CSR_LOCATIONS::MCAUSE, cause);
	csrs.write(CSR_LOCATIONS::MTVAL, trap.tval);

	PC = csrs.read(CSR_LOCATIONS::MTVEC) & ~0x3;
}

std::optional<Trap> CPU::load8(uint32_t address, uint8_t& value)
{
	switch (memory.read8(address, value))
	{
	case MemoryError::None:
		return std::nullopt;

	case MemoryError::AccessFault:
		return Trap::LoadAccessFault(address);

	case MemoryError::Misaligned:
		return Trap::LoadMisaligned(address);
	}

	return std::nullopt;
}

std::optional<Trap> CPU::load16(uint32_t address, uint16_t& value)
{
	switch (memory.read16(address, value))
	{
	case MemoryError::None:
		return std::nullopt;

	case MemoryError::AccessFault:
		return Trap::LoadAccessFault(address);

	case MemoryError::Misaligned:
		return Trap::LoadMisaligned(address);
	}

	return std::nullopt;
}

std::optional<Trap> CPU::load32(uint32_t address, uint32_t& value)
{
	switch (memory.read32(address, value))
	{
	case MemoryError::None:
		return std::nullopt;

	case MemoryError::AccessFault:
		return Trap::LoadAccessFault(address);

	case MemoryError::Misaligned:
		return Trap::LoadMisaligned(address);
	}

	return std::nullopt;
}

std::optional<Trap> CPU::store8(uint32_t address, uint8_t value)
{
	switch (memory.write8(address, value))
	{
	case MemoryError::None:
		return std::nullopt;

	case MemoryError::AccessFault:
		return Trap::StoreAccessFault(address);

	case MemoryError::Misaligned:
		return Trap::StoreMisaligned(address);
	}

	return std::nullopt;
}

std::optional<Trap> CPU::store16(uint32_t address, uint16_t value)
{
	switch (memory.write16(address, value))
	{
	case MemoryError::None:
		return std::nullopt;

	case MemoryError::AccessFault:
		return Trap::StoreAccessFault(address);

	case MemoryError::Misaligned:
		return Trap::StoreMisaligned(address);
	}

	return std::nullopt;
}

std::optional<Trap> CPU::store32(uint32_t address, uint32_t value)
{
	switch (memory.write32(address, value))
	{
	case MemoryError::None:
		return std::nullopt;

	case MemoryError::AccessFault:
		return Trap::StoreAccessFault(address);

	case MemoryError::Misaligned:
		return Trap::StoreMisaligned(address);
	}

	return std::nullopt;
}