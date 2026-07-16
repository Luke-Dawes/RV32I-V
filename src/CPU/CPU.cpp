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
	
	result.instruction = memory.read32(PC);
		
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

		Instructions[key](*this, d);
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