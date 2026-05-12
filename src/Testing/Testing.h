#pragma once
#include <cstdint>

class CPU;

//------------- Main testing function -------------------

void run_tests(CPU& cpu);


// ------------- S-type ----------------------------------

void test_sb(CPU& cpu);
void test_sh(CPU& cpu);
void test_sw(CPU& cpu);



// ------------- R-type ----------------------------------

void test_add(CPU& cpu);
void test_sub(CPU& cpu);
void test__xor(CPU& cpu);
void test__or(CPU& cpu);
void test__and(CPU& cpu);
void test_sll(CPU& cpu);
void test_srl(CPU& cpu);
void test_sra(CPU& cpu);
void test_slt(CPU& cpu);
void test_sltu(CPU& cpu);