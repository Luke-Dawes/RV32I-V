#pragma once
#include <cstdint>

class CPU;
struct Decoded_instruction;

void add(CPU& cpu, Decoded_instruction& ins);
void sub(CPU& cpu, Decoded_instruction& ins);
void _xor(CPU& cpu, Decoded_instruction& ins);
void _or(CPU& cpu, Decoded_instruction& ins);
void _and(CPU& cpu, Decoded_instruction& ins);
void sll(CPU& cpu, Decoded_instruction& ins);
void srl(CPU& cpu, Decoded_instruction& ins);
void sra(CPU& cpu, Decoded_instruction& ins);
void slt(CPU& cpu, Decoded_instruction& ins);
void sltu(CPU& cpu, Decoded_instruction& ins);

void mul(CPU& cpu, Decoded_instruction& ins);
void mulh(CPU& cpu, Decoded_instruction& ins);
void mulhu(CPU& cpu, Decoded_instruction& ins);
void div(CPU& cpu, Decoded_instruction& ins);
void divu(CPU& cpu, Decoded_instruction& ins);
void rem(CPU& cpu, Decoded_instruction& ins);
void remu(CPU& cpu, Decoded_instruction& ins);
