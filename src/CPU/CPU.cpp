#include "CPU.h"
#include "../Memory/Memory.h"
#include "CSR.h"
#include <iostream>

CPU::CPU(Memory& mem) : memory(mem) {}

uint32_t CPU::fetch() {
	if (PC < memory.RAM_BASE || PC - memory.RAM_BASE > memory.memory_size - 4) {
		//throw error
		throw out_of_bounds; //this is terminal so i dont think it can catch
	}

	if (PC % 4 != 0) {
		throw miss_aligned_trap;
	}

	/*std::cout << "PC=" << std::hex << PC << " ";
	std::cout << "bytes="
		<< (int)memory.RAM[PC] << " "
		<< (int)RAM[PC + 1] << " "
		<< (int)RAM[PC + 2] << " "
		<< (int)RAM[PC + 3] << std::endl;
	*/
	
	uint32_t data = memory.read32(PC);
		
	return data;
}

Decoded_instruction CPU::decode(uint32_t ins) {
	return decode_ins(ins);
}


void CPU::execute(Decoded_instruction d) {

	std::cout << "PC = " << std::hex << PC << std::endl;

	uint32_t key = make_key(d.funct7, d.funct3, d.opcode);

	/*std::cout << std::hex
		<< "key=" << key
		<< " op=" << (int)d.opcode
		<< " f3=" << (int)d.funct3
		<< " f7=" << (int)d.funct7
		<< " b30=" << (int)d.b30
		<< std::endl;*/

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
		raise_trap(2, d.full); // Illegal instruction
	}
}

void CPU::tick() { 
	branch_happended = false;
	uint32_t ins = fetch();

	std::cout << "RAW INSTRUCTION: 0x"
		<< std::hex
		<< ins
		<< "\n";

	Decoded_instruction d = decode(ins);

	std::cout
		<< " rd=x" << static_cast<int>(d.rd)
		<< " rs1=x" << static_cast<int>(d.rs1)
		<< " rs1_val=0x" << std::hex << registers[d.rs1]
		<< " imm=0x" << d.imm
		<< std::dec << "\n";
	 
	execute(d);

	if (!branch_happended) {
		PC += 4;
	}

	/*std::cout << "PC: " << PC
		<< " | x1: " << registers[1]
		<< " | x2: " << registers[2]
		<< " | x3: " << registers[3]
		<< " | x5: " << registers[5] 
		<< "\n";

	std::cout << "\n";*/
}

void CPU::raise_trap(uint32_t cause, uint32_t tval) {

	std::cout << "\n\n raising trap \n\n";

	csrs.write(CSR_LOCATIONS::MEPC, PC);
	csrs.write(CSR_LOCATIONS::MCAUSE, cause);
	csrs.write(CSR_LOCATIONS::MTVAL, tval);

	PC = csrs.read(CSR_LOCATIONS::MTVEC);
}