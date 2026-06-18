#include "Testing.h"
#include "../Instructions/R-type/R-Instructions.h"
#include "../Instructions/S-type/S-Instructions.h"
#include "../Instructions/I-type/I-Instructions.h"
#include "../Instructions/B, U, J-type/B,J,U-instructions.h"
#include <cassert>
#include <cstring>
#include "../CPU/Memory.h"
#include "../CPU/CPU.h"
#include <string>
#include <iostream>

//------------- Main testing function -------------------

void run_cpu_tests(CPU& cpu) {
	test_program_execution(cpu);
	test_add_five_without_branch_rv32i(cpu);
	test_add_five_with_branch_rv32i(cpu);
	test_fibonacci_real_rv32i(cpu);
}

// ------------- S-type ----------------------------------

void test_program_execution(CPU& cpu)
{
	// Clean start only once
	std::memset(RAM, 0, 0x100);

	for (int i = 0; i < 32; i++)
		cpu.registers[i] = 0;

	cpu.PC = 0x1000;

	Decoded_instruction d;

	//-----------------------------------
	// x1 = 10
	//-----------------------------------

	d.rd = 1;
	d.rs1 = 0;
	d.imm = 10;

	addi(cpu, d);

	//-----------------------------------
	// x2 = 20
	//-----------------------------------

	d.rd = 2;
	d.rs1 = 0;
	d.imm = 20;

	addi(cpu, d);

	//-----------------------------------
	// x3 = x1 + x2 = 30
	//-----------------------------------

	d.rd = 3;
	d.rs1 = 1;
	d.rs2 = 2;

	add(cpu, d);

	//-----------------------------------
	// x4 = x3 - x1 = 20
	//-----------------------------------

	d.rd = 4;
	d.rs1 = 3;
	d.rs2 = 1;

	sub(cpu, d);

	//-----------------------------------
	// x5 = x4 << 2
	//-----------------------------------

	cpu.registers[6] = 2;

	d.rd = 5;
	d.rs1 = 4;
	d.rs2 = 6;

	sll(cpu, d);

	//-----------------------------------
	// x7 = x5 | 3
	//-----------------------------------

	d.rd = 7;
	d.rs1 = 5;
	d.imm = 3;

	ori(cpu, d);

	//-----------------------------------
	// store x7 to memory
	//-----------------------------------

	cpu.registers[8] = 100;

	d.rs1 = 8;
	d.rs2 = 7;
	d.imm = 0;

	sw(cpu, d);

	//-----------------------------------
	// load back
	//-----------------------------------

	d.rd = 9;
	d.rs1 = 8;
	d.imm = 0;

	lw(cpu, d);

	//-----------------------------------
	// signed compare
	//-----------------------------------

	d.rd = 10;
	d.rs1 = 1;
	d.rs2 = 2;

	slt(cpu, d);

	//-----------------------------------
	// branch test
	//-----------------------------------

	cpu.PC = 0x1000;

	d.rs1 = 9;
	d.rs2 = 7;
	d.imm = 16;

	beq(cpu, d);

	//-----------------------------------
	// jal
	//-----------------------------------

	d.rd = 11;
	d.imm = 20;

	jal(cpu, d);

	//-----------------------------------
	// Final assertions
	//-----------------------------------

	assert(cpu.registers[1] == 10);

	assert(cpu.registers[2] == 20);

	assert(cpu.registers[3] == 30);

	assert(cpu.registers[4] == 20);

	assert(cpu.registers[5] == 80);

	assert(cpu.registers[7] == 83);

	assert(cpu.registers[9] == 83);

	assert(cpu.registers[10] == 1);

	assert(cpu.PC == 0x1024);

	assert(cpu.registers[11] == 0x1014);

	assert(cpu.registers[0] == 0);
}

void test_fibonacci_real_rv32i(CPU& cpu)
{
	memset(RAM, 0, 0x100);

	for (int i = 0; i < 32; i++)
		cpu.registers[i] = 0;

	cpu.PC = 0;

	uint32_t fib_program[] = {
		// --- Initialization Phase ---
		0x00000093, // PC = 0x00:  addi x1, x0, 0   (x1 = 0 + 0)
		0x00100113, // PC = 0x04:  addi x2, x0, 1   (x2 = 0 + 1)
		0x00A00293, // PC = 0x08:  addi x5, x0, 10  (x5 = 0 + 10)

		// --- Loop Body ---
		0x002081B3, // PC = 0x0C:  add x3, x1, x2   (x3 = x1 + x2)
		0x00010093, // PC = 0x10:  addi x1, x2, 0   (x1 = x2 + 0)
		0x00018113, // PC = 0x14:  addi x2, x3, 0   (x2 = x3 + 0)
		0xFFF28293, // PC = 0x18:  addi x5, x5, -1  (x5 = x5 + -1)

		// --- Loop Control & Exit ---
		0xFE0298E3, // PC = 0x1C:  bne x5, x0, -16  (if x5 != x0, jump to PC 0x0C)
		0x0000006F  // PC = 0x20:  jal x0, 0        (jump to PC 0x20, trap loop)
	};

	// Load program into RAM
	for (int i = 0; i < 9; i++) {
		uint32_t inst = fib_program[i];
		RAM[i * 4 + 0] = (uint8_t)(inst & 0xFF);         // Lowest byte
		RAM[i * 4 + 1] = (uint8_t)((inst >> 8) & 0xFF);
		RAM[i * 4 + 2] = (uint8_t)((inst >> 16) & 0xFF);
		RAM[i * 4 + 3] = (uint8_t)((inst >> 24) & 0xFF); // Highest byte
	}

	// Run enough cycles
	for (int i = 0; i < 50; i++)
		cpu.tick();

	//remember cpu.registers is printed via a hex number, meaning that it wont be 89
	assert(cpu.registers[3] == 89 && "fib register 3 fail");
	assert(cpu.registers[1] == 55 && "fib register 1 fail");
}

void test_add_five_with_branch_rv32i(CPU& cpu)
{
	memset(RAM, 0, 0x100);

	for (int i = 0; i < 32; i++)
		cpu.registers[i] = 0;

	cpu.PC = 0;

	// RISC-V RV32I Machine Code
	uint32_t add_program[] = {
		0x00500193, // PC=0:  li x3, 5          (Initialize loop counter x3 = 5)
		// --- LOOP TARGET BEGINS HERE ---
		0x00108093, // PC=4:  addi x1, x1, 1    (Increment target x1 by 1)
		0xFFF18193, // PC=8:  addi x3, x3, -1   (Decrement loop counter x3)
		0xFE019CE3, // PC=C:  bne x3, x0, -8    (If x3 != 0, jump back to PC=4)
		0x0000006F  // PC=10: jal x0, 0         (Halt infinite loop)
	};

	// Load program into RAM
	for (int i = 0; i < 5; i++) {
		uint32_t inst = add_program[i];
		RAM[i * 4 + 0] = (uint8_t)(inst & 0xFF);
		RAM[i * 4 + 1] = (uint8_t)((inst >> 8) & 0xFF);
		RAM[i * 4 + 2] = (uint8_t)((inst >> 16) & 0xFF);
		RAM[i * 4 + 3] = (uint8_t)((inst >> 24) & 0xFF);
	}

	// Run enough cycles
	for (int i = 0; i < 30; i++)
		cpu.tick();

	// Expected result check
	std::string error_msg = "Add test failed (x1 should be 5) but is " + std::to_string(cpu.registers[1]);
	assert(cpu.registers[1] == 5 && error_msg.c_str());
}

void test_add_five_without_branch_rv32i(CPU& cpu)
{
	memset(RAM, 0, 0x100);

	for (int i = 0; i < 32; i++)
		cpu.registers[i] = 0;

	cpu.PC = 0;

	// RISC-V RV32I Machine Code
	uint32_t add_program[] = {
		0x00108093, // addi x1, x1, 1 (1st add)
		0x00108093, // addi x1, x1, 1 (2nd add)
		0x00108093, // addi x1, x1, 1 (3rd add)
		0x00108093, // addi x1, x1, 1 (4th add)
		0x00108093, // addi x1, x1, 1 (5th add)
		0x0000006F  // jal x0, 0      (Halt/Jump to self permanently)
	};

	// Load program into RAM
	for (int i = 0; i < 6; i++) {
		uint32_t inst = add_program[i];
		RAM[i * 4 + 0] = (uint8_t)(inst & 0xFF);
		RAM[i * 4 + 1] = (uint8_t)((inst >> 8) & 0xFF);
		RAM[i * 4 + 2] = (uint8_t)((inst >> 16) & 0xFF);
		RAM[i * 4 + 3] = (uint8_t)((inst >> 24) & 0xFF);
	}

	// Run enough cycles
	for (int i = 0; i < 7; i++)
		cpu.tick();

	// Expected result check
	std::string error_msg = "Add test failed (x1 should be 5) but is " + std::to_string(cpu.registers[1]);
	assert(cpu.registers[1] == 5 && error_msg.c_str());
	std::cout << "PC=" << std::hex << cpu.PC << "\n";
	assert(cpu.PC == 0x14 && "PC moved above 14 when it should be stuck there");
	std::cout << "test_add_five_without_branch worked \n\n\n\n";
}