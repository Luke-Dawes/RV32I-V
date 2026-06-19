#pragma once


class CPU;
struct Decoded_instruction;



// ----- B type ----------
void beq(CPU& cpu, Decoded_instruction& ins);
void bne(CPU& cpu, Decoded_instruction& ins);
void blt(CPU& cpu, Decoded_instruction& ins);
void bge(CPU& cpu, Decoded_instruction& ins);
void bltu(CPU& cpu, Decoded_instruction& ins);
void bgeu(CPU& cpu, Decoded_instruction& ins);

// ----- J type ----------
void jal(CPU& cpu, Decoded_instruction& ins);

// ----- U type ----------
void lui(CPU& cpu, Decoded_instruction& ins);
void auipc(CPU& cpu, Decoded_instruction& ins);