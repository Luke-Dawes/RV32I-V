#pragma once
#include <cstdint>

class CPU;
struct Decoded_instruction;

void sb(CPU& cpu, Decoded_instruction& ins);
void sh(CPU& cpu, Decoded_instruction& ins);
void sw(CPU& cpu, Decoded_instruction& ins);
