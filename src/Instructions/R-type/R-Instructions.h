#pragma once
#include <cstdint>
#include "../../CPU/Memory.h"

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