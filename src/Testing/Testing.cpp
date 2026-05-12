#include "Testing.h"
#include "../Instructions/R-type/R-Instructions.h"
#include "../Instructions/S-type/S-Instructions.h"
#include <cassert>
#include "../CPU/Memory.h"
#include "../CPU/CPU.h"

//------------- Main testing function -------------------

void run_tests(CPU& cpu) {

	// s types
	test_sb(cpu);

	//r types
	test_add(cpu);
	test_sub(cpu);

}

// ------------- S-type ----------------------------------

void test_sb(CPU& cpu)
{
	cpu.registers[5] = 5; // Base address
	cpu.registers[6] = 6; // Value to store

	Decoded_instruction d;

	d.rs1 = 5; //this
	d.imm = 5; //plus this for the location
	d.rs2 = 6; //value to be stored
	

	sb(cpu, d);
	//should store it at the 10th ram address
	
	assert(RAM[10] == 6 && "loading 6 into ram addess 10 on test_sb failed");

}

void test_sh(CPU& cpu);
void test_sw(CPU& cpu);








//need loading first
void test_add(CPU& cpu)
{
	cpu.registers[5] = 2;
	cpu.registers[6] = 4;

	Decoded_instruction d;
	d.rs1 = 5;
	d.rs2 = 6;
	d.rd = 0;

	add(cpu, d); //add whatever is in register 5 and 6 to 0
	
	assert(cpu.registers[0] == 0 && "register 0 had its value changes in test_add");

	d.rd = 1;

	add(cpu, d);

	assert(cpu.registers[1] == 6 && "addition broke in test_add");

}
void test_sub(CPU& cpu)
{
	cpu.registers[5] = 2;
	cpu.registers[6] = 4;

	Decoded_instruction d;
	d.rs1 = 6;
	d.rs2 = 5;
	d.rd = 0;

	sub(cpu, d); 

	assert(cpu.registers[0] == 0 && "register 0 had its value changes in test_sub");

	d.rd = 1;

	sub(cpu, d);

	assert(cpu.registers[1] == 2 && "subtraction broke in test_sub");

	d.rd = 2;
	d.rs1 = 5;
	d.rs2 = 6;

	sub(cpu, d);

	assert(cpu.registers[2] == static_cast<uint32_t>(-2) && "underflow broke in test_sub");

}
void test__xor(CPU& cpu);
void test__or(CPU& cpu);
void test__and(CPU& cpu);
void test_sll(CPU& cpu);
void test_srl(CPU& cpu);
void test_sra(CPU& cpu);
void test_slt(CPU& cpu);
void test_sltu(CPU& cpu);