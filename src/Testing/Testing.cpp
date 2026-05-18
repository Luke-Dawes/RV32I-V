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

//------------- Main testing function -------------------

void run_tests(CPU& cpu) {

	// s types
	test_sb(cpu);
	test_sh(cpu);
	test_sw(cpu);

	//r types
	test_add(cpu);
	test_sub(cpu);
	test__xor(cpu);
	test__and(cpu);
	test__or(cpu);
	test_sll(cpu);
	test_srl(cpu);
	test_sra(cpu);
	test_slt(cpu);
	test_sltu(cpu);
	test_addi(cpu);
	test_slti(cpu);
	test_sltiu(cpu);
	test_xori(cpu);
	test_ori(cpu);
	test_andi(cpu);
	test_slli(cpu);
	test_srli(cpu);
	test_srai(cpu);
	test_lb(cpu);
	test_lh(cpu);
	test_lw(cpu);
	test_lbu(cpu);
	test_lhu(cpu);
	test_beq(cpu);
	test_bne(cpu);
	test_blt(cpu);
	test_bge(cpu);
	test_bltu(cpu);
	test_bgeu(cpu);
	test_lui(cpu);
	test_auipc(cpu);
	test_jal(cpu);
	test_jalr(cpu);
	test_program_execution(cpu);
	test_fibonacci_real_rv32i(cpu);



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
	

	sb(cpu, d); //register[rs1] + imm for the location, register[rs2] for data
	//should store it at the 10th ram address
	
	assert(RAM[10] == 6 && "loading 6 into ram addess 10 on test_sb failed");

}

void test_sh(CPU& cpu) {
	cpu.registers[5] = 10;
	cpu.registers[6] = 0x1122;

	Decoded_instruction d;
	d.rs1 = 5;
	d.imm = 10;
	d.rs2 = 6;

	sh(cpu, d); //register[rs1] + imm for the location, register[rs2] for data

	uint32_t addr = 20;

	assert(RAM[addr] == 0x22 && "test_sh failed first byte");
	assert(RAM[addr + 1] == 0x11 && "test_sh failed second byte");
}

void test_sw(CPU& cpu) {
	cpu.registers[5] = 10;
	cpu.registers[6] = 0x11223344;

	Decoded_instruction d;
	d.rs1 = 5;
	d.imm = 10;
	d.rs2 = 6;

	sw(cpu, d); //register[rs1] + imm for the location, register[rs2] for data

	uint32_t addr = 20;

	assert(RAM[addr] == 0x44 && "test_sh failed first byte");
	assert(RAM[addr + 1] == 0x33 && "test_sh failed second byte");
	assert(RAM[addr + 2] == 0x22 && "test_sh failed third byte");
	assert(RAM[addr + 3] == 0x11 && "test_sh failed fourth byte");
}

// ------------- R-type ----------------------------------

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

void test__xor(CPU& cpu) {
	cpu.registers[5] = 5; //101
	cpu.registers[6] = 1; //001

	Decoded_instruction d;
	d.rs1 = 5;
	d.rs2 = 6;
	d.rd = 1;

	_xor(cpu, d);

	assert(cpu.registers[1] == 4 && "xor broke in test_xor");
}

void test__or(CPU& cpu) {
	cpu.registers[5] = 5; //101
	cpu.registers[6] = 1; //001

	Decoded_instruction d;
	d.rs1 = 5;
	d.rs2 = 6;
	d.rd = 1;

	_or(cpu, d);

	assert(cpu.registers[1] == 5 && "or broke in test_or");
}

void test__and(CPU& cpu) {
	cpu.registers[5] = 5; //101
	cpu.registers[6] = 1; //001

	Decoded_instruction d;
	d.rs1 = 5;
	d.rs2 = 6;
	d.rd = 1;

	_and(cpu, d);

	assert(cpu.registers[1] == 1 && "and broke in test_and");
}

void test_sll(CPU& cpu) {
	cpu.registers[5] = 5; //101
	cpu.registers[6] = 1; //001

	Decoded_instruction d;
	d.rs1 = 5;
	d.rs2 = 6;
	d.rd = 1;

	sll(cpu, d);

	assert(cpu.registers[1] == 10 && "shifting right stopped working in test_sll");
}

void test_srl(CPU& cpu) {
	cpu.registers[5] = 5; //101
	cpu.registers[6] = 1; //001

	Decoded_instruction d;
	d.rs1 = 5;
	d.rs2 = 6;
	d.rd = 1;

	srl(cpu, d);

	assert(cpu.registers[1] == 2 && "shifting left stopped working in test_srl");
}

void test_sra(CPU& cpu) {
	// 0x80000000 has the sign bit set (negative value)
	cpu.registers[5] = 0x80000000;
	cpu.registers[6] = 1;

	Decoded_instruction d;
	d.rs1 = 5;
	d.rs2 = 6;
	d.rd = 1;

	sra(cpu, d);

	// Arithmetic shift right must preserve the sign bit (MSB), resulting in 0xC0000000
	assert(cpu.registers[1] == 0xC0000000 && "arithmetic right shift failed to preserve sign bit in test_sra");
}

void test_slt(CPU& cpu) {
	// Signed evaluation: -5 is less than 2
	cpu.registers[5] = static_cast<uint32_t>(-5);
	cpu.registers[6] = 2;

	Decoded_instruction d;
	d.rs1 = 5;
	d.rs2 = 6;
	d.rd = 1;

	slt(cpu, d);

	assert(cpu.registers[1] == 1 && "signed set less than failed true condition in test_slt");

	// Switch values: 2 is not less than -5
	cpu.registers[5] = 2;
	cpu.registers[6] = static_cast<uint32_t>(-5);
	slt(cpu, d);

	assert(cpu.registers[1] == 0 && "signed set less than failed false condition in test_slt");
}

void test_sltu(CPU& cpu) {
	// Unsigned evaluation: 2 is less than 0xFFFFFFFB (-5 interpreted as unsigned)
	cpu.registers[5] = 2;
	cpu.registers[6] = static_cast<uint32_t>(-5);

	Decoded_instruction d;
	d.rs1 = 5;
	d.rs2 = 6;
	d.rd = 1;

	sltu(cpu, d);

	assert(cpu.registers[1] == 1 && "unsigned set less than failed true condition in test_sltu");
}

// ------------- I-type (Arithmetic & Logical Immediates) ----------

void test_addi(CPU& cpu) {
	cpu.registers[5] = 10;

	Decoded_instruction d;
	d.rs1 = 5;
	d.imm = -4; // Tests negative sign extension handling
	d.rd = 1;

	addi(cpu, d);
	assert(cpu.registers[1] == 6 && "addi failed basic addition");

	d.rd = 0;
	addi(cpu, d);
	assert(cpu.registers[0] == 0 && "addi mutated register x0");
}

void test_slti(CPU& cpu) {
	cpu.registers[5] = static_cast<uint32_t>(-5);

	Decoded_instruction d;
	d.rs1 = 5;
	d.imm = -2; // -5 < -2 is true
	d.rd = 1;

	slti(cpu, d);
	assert(cpu.registers[1] == 1 && "slti failed true signed comparison");

	d.imm = -10; // -5 < -10 is false
	slti(cpu, d);
	assert(cpu.registers[1] == 0 && "slti failed false signed comparison");
}

void test_sltiu(CPU& cpu) {
	cpu.registers[5] = 2;

	Decoded_instruction d;
	d.rs1 = 5;
	d.imm = -5; // -5 as unsigned is 0xFFFFFFFB. 2 < 0xFFFFFFFB is true
	d.rd = 1;

	sltiu(cpu, d);
	assert(cpu.registers[1] == 1 && "sltiu failed unsigned immediate comparison");
}

void test_xori(CPU& cpu) {
	cpu.registers[5] = 5; // 0101

	Decoded_instruction d;
	d.rs1 = 5;
	d.imm = 1; // 0001
	d.rd = 1;

	xori(cpu, d);
	assert(cpu.registers[1] == 4 && "xori failed");
}

void test_ori(CPU& cpu) {
	cpu.registers[5] = 4; // 0100

	Decoded_instruction d;
	d.rs1 = 5;
	d.imm = 2; // 0010
	d.rd = 1;

	ori(cpu, d);
	assert(cpu.registers[1] == 6 && "ori failed");
}

void test_andi(CPU& cpu) {
	cpu.registers[5] = 7; // 0111

	Decoded_instruction d;
	d.rs1 = 5;
	d.imm = 2; // 0010
	d.rd = 1;

	andi(cpu, d);
	assert(cpu.registers[1] == 2 && "andi failed");
}

void test_slli(CPU& cpu) {
	cpu.registers[5] = 3; // 0011

	Decoded_instruction d;
	d.rs1 = 5;
	d.imm = 2; // Shift count
	d.rd = 1;

	slli(cpu, d);
	assert(cpu.registers[1] == 12 && "slli failed");
}

void test_srli(CPU& cpu) {
	cpu.registers[5] = 12;

	Decoded_instruction d;
	d.rs1 = 5;
	d.imm = 2; // Shift count
	d.rd = 1;

	srli(cpu, d);
	assert(cpu.registers[1] == 3 && "srli failed");
}

void test_srai(CPU& cpu) {
	cpu.registers[5] = 0x80000000; // Sign bit active

	Decoded_instruction d;
	d.rs1 = 5;
	d.imm = 1; // Shift count
	d.rd = 1;

	srai(cpu, d);
	assert(cpu.registers[1] == 0xC0000000 && "srai failed to preserve sign extension");
}

// ------------- I-type (Loads) ------------------------------------

void test_lb(CPU& cpu) {
	cpu.registers[5] = 10;
	RAM[12] = 0x85; // Negative byte value (1000 0101)

	Decoded_instruction d;
	d.rs1 = 5;
	d.imm = 2; // Target address 12
	d.rd = 1;

	lb(cpu, d);
	// lb performs sign extension to 32 bits
	assert(cpu.registers[1] == 0xFFFFFF85 && "lb failed to sign-extend loaded byte");
}

void test_lh(CPU& cpu) {
	cpu.registers[5] = 20;
	RAM[20] = 0x00;
	RAM[21] = 0x90; // High byte has sign bit active (Little Endian target 0x9000)

	Decoded_instruction d;
	d.rs1 = 5;
	d.imm = 0;
	d.rd = 1;

	lh(cpu, d);
	assert(cpu.registers[1] == 0xFFFF9000 && "lh failed to sign-extend loaded half-word");
}

void test_lw(CPU& cpu) {
	cpu.registers[5] = 30;
	RAM[34] = 0x44;
	RAM[35] = 0x33;
	RAM[36] = 0x22;
	RAM[37] = 0x11; // Memory holds 0x11223344

	Decoded_instruction d;
	d.rs1 = 5;
	d.imm = 4; // Target address 34
	d.rd = 1;

	lw(cpu, d);
	assert(cpu.registers[1] == 0x11223344 && "lw failed to extract complete 32-bit word");
}

void test_lbu(CPU& cpu) {
	cpu.registers[5] = 40;
	RAM[40] = 0x85;

	Decoded_instruction d;
	d.rs1 = 5;
	d.imm = 0;
	d.rd = 1;

	lbu(cpu, d);
	// lbu zero-extends, keeping the upper register spaces clean
	assert(cpu.registers[1] == 0x00000085 && "lbu mistakenly sign-extended instead of zero-extending");
}

void test_lhu(CPU& cpu) {
	cpu.registers[5] = 50;
	RAM[50] = 0x22;
	RAM[51] = 0xBB; // 0xBB22

	Decoded_instruction d;
	d.rs1 = 5;
	d.imm = 0;
	d.rd = 1;

	lhu(cpu, d);
	assert(cpu.registers[1] == 0x0000BB22 && "lhu mistakenly sign-extended instead of zero-extending");
}

// ------------- B-type (Branches) ---------------------------------

void test_beq(CPU& cpu) {
	cpu.registers[5] = 42;
	cpu.registers[6] = 42;
	cpu.PC = 0x1000;

	Decoded_instruction d;
	d.rs1 = 5;
	d.rs2 = 6;
	d.imm = 16; // Jump offset

	// Condition is true: 42 == 42
	beq(cpu, d);
	assert(cpu.PC == 0x1010 && "beq failed when branch condition was true");

	// Condition is false: 42 == 0
	cpu.registers[5] = 42;
	cpu.registers[6] = 0;
	cpu.PC = 0x1000;

	beq(cpu, d);
	assert(cpu.PC == 0x1000 && "beq mistakenly branched when condition was false");
}

void test_bne(CPU& cpu) {
	cpu.registers[5] = 42;
	cpu.registers[6] = 0;
	cpu.PC = 0x1000;

	Decoded_instruction d;
	d.rs1 = 5;
	d.rs2 = 6;
	d.imm = 12;

	bne(cpu, d);
	assert(cpu.PC == 0x100C && "bne failed when branch condition was true");
}

void test_blt(CPU& cpu) {
	// Signed evaluation: -5 is less than 2
	cpu.registers[5] = static_cast<uint32_t>(-5);
	cpu.registers[6] = 2;
	cpu.PC = 0x1000;

	Decoded_instruction d;
	d.rs1 = 5;
	d.rs2 = 6;
	d.imm = 8;

	blt(cpu, d);
	assert(cpu.PC == 0x1008 && "blt failed a true signed comparison (-5 < 2)");
}

void test_bge(CPU& cpu) {
	// Signed evaluation: 2 is greater than or equal to -5
	cpu.registers[5] = 2;
	cpu.registers[6] = static_cast<uint32_t>(-5);
	cpu.PC = 0x1000;

	Decoded_instruction d;
	d.rs1 = 5;
	d.rs2 = 6;
	d.imm = 4;

	bge(cpu, d);
	assert(cpu.PC == 0x1004 && "bge failed a true signed comparison (2 >= -5)");
}

void test_bltu(CPU& cpu) {
	// Unsigned evaluation: 2 is less than 0xFFFFFFFB (-5 interpreted as unsigned)
	cpu.registers[5] = 2;
	cpu.registers[6] = static_cast<uint32_t>(-5);
	cpu.PC = 0x1000;

	Decoded_instruction d;
	d.rs1 = 5;
	d.rs2 = 6;
	d.imm = 20;

	bltu(cpu, d);
	assert(cpu.PC == 0x1014 && "bltu failed a true unsigned comparison (2 < 0xFFFFFFFB)");
}

void test_bgeu(CPU& cpu) {
	// Unsigned evaluation: 0xFFFFFFFB is greater than or equal to 2
	cpu.registers[5] = static_cast<uint32_t>(-5);
	cpu.registers[6] = 2;
	cpu.PC = 0x1000;

	Decoded_instruction d;
	d.rs1 = 5;
	d.rs2 = 6;
	d.imm = 24;

	bgeu(cpu, d);
	assert(cpu.PC == 0x1018 && "bgeu failed a true unsigned comparison (0xFFFFFFFB >= 2)");
}

// ------------- U-type (Upper Immediates) -------------------------

void test_lui(CPU& cpu) {

	Decoded_instruction d;
	d.imm = 0x12345; // 20-bit raw upper immediate value
	d.rd = 1;

	lui(cpu, d);
	// If your decode shifts it: check if it equals 0x12345000.
	// If your lui shifts it: the assertion below matches your prompt's "ins.imm << 12"
	assert(cpu.registers[1] == 0x12345000 && "lui failed to load upper immediate into register");
}

void test_auipc(CPU& cpu) {
	cpu.PC = 0x2000;

	Decoded_instruction d;
	d.imm = 0x12345;
	d.rd = 1;

	auipc(cpu, d);
	// Adds upper shifted immediate directly to current PC value
	assert(cpu.registers[1] == (0x2000 + 0x12345000) && "auipc failed to add upper immediate to PC");
}

// ------------- J-type (Jumps) ------------------------------------

void test_jal(CPU& cpu) {
	cpu.PC = 0x1000;

	Decoded_instruction d;
	d.imm = 32; // Jump target offset
	d.rd = 1;   // Link register

	jal(cpu, d);
	assert(cpu.PC == 0x1020 && "jal failed to jump to the correct target address");
	assert(cpu.registers[1] == 0x1004 && "jal failed to store the return address (PC + 4) in rd");
}

void test_jalr(CPU& cpu) {
	cpu.registers[5] = 0x2000; // Base jump address
	cpu.PC = 0x1000;

	Decoded_instruction d;
	d.rs1 = 5;
	d.imm = 4; // Offset added to base register
	d.rd = 1;  // Link register

	jalr(cpu, d);
	// jalr targets (cpu.registers[rs1] + imm) & ~1 (forces alignment clear on lowest bit)
	assert(cpu.PC == 0x2004 && "jalr failed to jump to target address (rs1 + imm)");
	assert(cpu.registers[1] == 0x1004 && "jalr failed to store return address (PC + 4) in rd");
}

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
		0x00000093,
		0x00100113,
		0x00A00293,

		0x002081B3,
		0x00010113,
		0x00018293,
		0xFFF28293,

		0xFE519CE3,
		0x0000006F
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

	// Expected Fibonacci result

	std::string error2 = "fib failed (x2 should be 89) but is " + std::to_string(cpu.registers[2]);
	std::string error1 = "fib failed (x1 should be 55) but is " + std::to_string(cpu.registers[1]);

	assert(cpu.registers[2] == 89 && error2.c_str());
	assert(cpu.registers[1] == 55 && error1.c_str());
}