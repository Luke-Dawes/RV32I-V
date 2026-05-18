#include "CPU.h"
#include "Memory.h"
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

#define IDX(b30, f3, op) (((b30) << 10) | ((f3) << 7) | (op))

	std::cout << "PC = " << std::hex << PC << std::endl;

	auto key = IDX((d.funct7 >> 5) & 1, d.funct3, d.opcode);

	std::cout << std::hex
		<< "key=" << key
		<< " op=" << d.opcode
		<< " f3=" << d.funct3
		<< " f7=" << d.funct7
		<< std::endl;

	if (Instructions[key] != nullptr) {
		std::cout << "instruction is " << Instructions[key] << "\n";
		Instructions[key](*this, d);
	}
	else {
		std::cout << "unknown instruction, key=" << key << "\n";
	}
}

void CPU::tick() { //poor set up
	uint32_t ins = fetch();

	Decoded_instruction d = decode(ins);

	uint32_t oldPC = PC;
	 
	execute(d);

	if (PC == oldPC) {
		PC += 4;
	}

	std::cout << "PC: " << PC
		<< " | x1: " << registers[1]
		<< " | x2: " << registers[2]
		<< " | x5: " << registers[5] << "\n";

	std::cout << "\n";
}