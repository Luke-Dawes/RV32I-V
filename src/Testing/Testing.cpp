#include "Testing.h"
#include "../Instructions/R-type/R-Instructions.h"
#include "../Instructions/S-type/S-Instructions.h"
#include <cassert>
#include "../CPU/Memory.h"

//------------- Main testing function -------------------

void run_tests() {
	test_sb();
}

// ------------- S-type ----------------------------------

void test_sb() 
{
	sb(5, 5, 6, 0);
	//should store it at the 10th ram address
	
	assert(RAM[10] == 6 && "loading 6 into ram addess 10 on test_sb failed");
}

void test_sh();
void test_sw();








//need loading first
void test_add() 
{
	add(0, 5, 6, 0); //add whatever is in register 5 and 6 to 0

}
void test_sub();
void test__xor();
void test__or();
void test__and();
void test_sll();
void test_srl();
void test_sra();
void test_slt();
void test_sltu();