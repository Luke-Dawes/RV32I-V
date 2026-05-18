#pragma once
#include <cstdint>

class CPU;
struct Decoded_instruction;



void addi(CPU& cpu, Decoded_instruction& ins);
void xori(CPU& cpu, Decoded_instruction& ins);
void ori(CPU& cpu, Decoded_instruction& ins);
void andi(CPU& cpu, Decoded_instruction& ins);
void slli(CPU& cpu, Decoded_instruction& ins);
void srli(CPU& cpu, Decoded_instruction& ins);
void srai(CPU& cpu, Decoded_instruction& ins);
void slti(CPU& cpu, Decoded_instruction& ins);
void sltiu(CPU& cpu, Decoded_instruction& ins);


void lb(CPU& cpu, Decoded_instruction& ins);
void lh(CPU& cpu, Decoded_instruction& ins);
void lw(CPU& cpu, Decoded_instruction& ins);
void lbu(CPU& cpu, Decoded_instruction& ins);
void lhu(CPU& cpu, Decoded_instruction& ins);

void jalr(CPU& cpu, Decoded_instruction& ins);
void ecall(CPU& cpu, Decoded_instruction& ins) {}
void ebreak(CPU& cpu, Decoded_instruction& ins) {}
