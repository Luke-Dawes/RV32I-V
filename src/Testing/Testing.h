#pragma once
#include <cstdint>

//------------- Main testing function -------------------

void run_tests();


// ------------- S-type ----------------------------------

void test_sb();
void test_sh();
void test_sw();



// ------------- R-type ----------------------------------

void test_add();
void test_sub();
void test__xor();
void test__or();
void test__and();
void test_sll();
void test_srl();
void test_sra();
void test_slt();
void test_sltu();