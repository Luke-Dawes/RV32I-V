#include "Instructions.h"
#include "../../CPU/Memory.h"
#include "../../CPU/Memory.h" //only way intellisense will work is include it twice

void add(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	RAM[rd] = RAM[rs1] + RAM[rs2];
}

void sub(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	RAM[rd] = RAM[rs1] - RAM[rs2];
}

void _xor(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	RAM[rd] = RAM[rs1] ^ RAM[rs2];
}

void _or(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	RAM[rd] = RAM[rs1] | RAM[rs2];
}

void _and(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	RAM[rd] = RAM[rs1] & RAM[rs2];
}

void sll(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	RAM[rd] = RAM[rs1] << RAM[rs2];
}

void srl(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	RAM[rd] = RAM[rs1] >> RAM[rs2];
}

//if the number is negative it the empty slots created by shifting are filled with 1s.
void sra(uint32_t rd, uint32_t rs1, uint32_t rs2) { 
	RAM[rd] = static_cast<int32_t>(RAM[rs1]) >> RAM[rs2];
}

//signed comparison
void slt(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	RAM[rd] = (static_cast<int32_t>(RAM[rs1]) < static_cast<int32_t>(RAM[rs2])) ? 1 : 0;
}

//unsigned comparision
void sltu(uint32_t rd, uint32_t rs1, uint32_t rs2) { //theres a note of zero-extends. Dont know 
	RAM[rd] = (RAM[rs1] < RAM[rs2]) ? 1 : 0;
}