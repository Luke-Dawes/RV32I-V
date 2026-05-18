#pragma once

class CPU;

// ------------- Main func ------------------------------
void run_tests(CPU& cpu);


// ------------- R-Type Tests ------------------------------
void test_add(CPU& cpu);
void test_sub(CPU& cpu);
void test_sll(CPU& cpu);
void test_slt(CPU& cpu);
void test_sltu(CPU& cpu);
void test__xor(CPU& cpu);
void test_srl(CPU& cpu);
void test_sra(CPU& cpu);
void test__or(CPU& cpu);
void test__and(CPU& cpu);

// ------------- I-Type Tests (Arithmetic & Logical) -------
void test_addi(CPU& cpu);
void test_slti(CPU& cpu);
void test_sltiu(CPU& cpu);
void test_xori(CPU& cpu);
void test_ori(CPU& cpu);
void test_andi(CPU& cpu);
void test_slli(CPU& cpu);
void test_srli(CPU& cpu);
void test_srai(CPU& cpu);

// ------------- I-Type Tests (Loads) ----------------------
void test_lb(CPU& cpu);
void test_lh(CPU& cpu);
void test_lw(CPU& cpu);
void test_lbu(CPU& cpu);
void test_lhu(CPU& cpu);

// ------------- S-Type Tests (Stores) ---------------------
void test_sb(CPU& cpu);
void test_sh(CPU& cpu);
void test_sw(CPU& cpu);

// ------------- B-Type Tests (Branches) -------------------
void test_beq(CPU& cpu);
void test_bne(CPU& cpu);
void test_blt(CPU& cpu);
void test_bge(CPU& cpu);
void test_bltu(CPU& cpu);
void test_bgeu(CPU& cpu);

// ------------- U-Type Tests (Upper Immediates) -----------
void test_lui(CPU& cpu);
void test_auipc(CPU& cpu);

// ------------- J-Type Tests (Jumps) ----------------------
void test_jal(CPU& cpu);
void test_jalr(CPU& cpu);

// ------------- Environment / System Tests ----------------
void test_ecall(CPU& cpu);
void test_ebreak(CPU& cpu);

void test_program_execution(CPU& cpu);
void test_fibonacci_real_rv32i(CPU& cpu);