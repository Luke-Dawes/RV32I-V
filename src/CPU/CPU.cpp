#include "CPU.h"
#include "../Memory/Memory.h"
#include <iostream>

CPU::CPU() {
	init_table();
}

uint32_t CPU::fetch() {
	if (PC + 3 >= 0x100) {
		//throw error
		throw out_of_bounds; //this is terminal so i dont think it can catch
	}

	if (PC % 4 != 0) {
		throw miss_aligned_trap;
	}

	std::cout << "PC=" << std::hex << PC << " ";
	std::cout << "bytes="
		<< (int)RAM[PC] << " "
		<< (int)RAM[PC + 1] << " "
		<< (int)RAM[PC + 2] << " "
		<< (int)RAM[PC + 3] << std::endl;

	uint32_t data = RAM[PC] |
		(RAM[PC + 1] << 8) |
		(RAM[PC + 2] << 16) |
		(RAM[PC + 3] << 24);

	return data;
}

Decoded_instruction CPU::decode(uint32_t ins) {
	return decode_ins(ins);
}


void CPU::execute(Decoded_instruction d) {

#define IDX(b30, f3, op) ((b30) | (((f3) & 0x7) << 7) | ((op) & 0x7F))

	std::cout << "PC = " << std::hex << PC << std::endl;

	uint32_t key = IDX(d.b30, d.funct3, d.opcode);

	std::cout << std::hex
		<< "key=" << key
		<< " op=" << (int)d.opcode
		<< " f3=" << (int)d.funct3
		<< " f7=" << (int)d.funct7
		<< " b30=" << (int)d.b30
		<< std::endl;

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
	}
}

void CPU::tick() { //poor set up
	branch_happended = false;
	uint32_t ins = fetch();

	Decoded_instruction d = decode(ins);
	 
	execute(d);

	if (!branch_happended) {
		PC += 4;
	}

	std::cout << "PC: " << PC
		<< " | x1: " << registers[1]
		<< " | x2: " << registers[2]
		<< " | x3: " << registers[3]
		<< " | x5: " << registers[5] 
		<< "\n";

	std::cout << "\n";
}