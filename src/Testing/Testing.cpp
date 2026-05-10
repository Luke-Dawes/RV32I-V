#include "Testing.h"
#include "../Instructions/R-type/R-Instructions.h"
#include "../Instructions/S-type/S-Instructions.h"
#include <cassert>
#include "../CPU/Memory.h"

//------------- Main testing function -------------------

void run_tests() {

	// s types
	test_sb();

	//r types
	test_add();
	test_sub();

}

// ------------- S-type ----------------------------------

void test_sb() 
{
	registers[5] = 5; // Base address
	registers[6] = 6; // Value to store
	

	sb(5, 5, 6, 0);
	//should store it at the 10th ram address
	
	assert(RAM[10] == 6 && "loading 6 into ram addess 10 on test_sb failed");

}

void test_sh();
void test_sw();








//need loading first
void test_add() 
{
	registers[5] = 2;
	registers[6] = 4;
	add(0, 5, 6, 0); //add whatever is in register 5 and 6 to 0
	
	assert(registers[0] == 0 && "register 0 had its value changes in test_add");


	add(1, 5, 6, 0);

	assert(registers[1] == 6 && "addition broke in test_add");

}
void test_sub() 
{
	registers[5] = 2;
	registers[6] = 4;
	sub(0, 5, 6, 0); 

	assert(registers[0] == 0 && "register 0 had its value changes in test_sub");

	sub(1, 6, 5, 0);

	assert(registers[1] == 2 && "subtraction broke in test_sub");

	sub(2, 5, 6, 0);

	assert(registers[2] == static_cast<uint32_t>(-2) && "underflow broke in test_sub");

}
void test__xor()
{
	//
	registers[5] 
	registers[6]
}
void test__or();
void test__and();
void test_sll();
void test_srl();
void test_sra();
void test_slt();
void test_sltu();