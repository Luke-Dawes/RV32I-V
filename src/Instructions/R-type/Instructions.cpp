#include "Instructions.h"
#include "../../CPU/Memory.h"
#include "../../CPU/Memory.h" //only way intellisense will work is include it twice


void add(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	RAM[rd] = RAM[rs1] + RAM[rs2];
}

void sub(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	RAM[rd] = RAM[rs1] - RAM[rs2];
}

void xor(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	RAM[rd] = RAM[rs1] ^ RAM[rs2];
}

void or(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	RAM[rd] = RAM[rs1] | RAM[rs2];
}

void and(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	RAM[rd] = RAM[rs1] & RAM[rs2];
}

void sll(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	RAM[rd] = RAM[rs1] << RAM[rs2];
}

void srl(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	RAM[rd] = RAM[rs1] >> RAM[rs2];
}

void sra(uint32_t rd, uint32_t rs1, uint32_t rs2) { //there a note of MSB-extends. I dont know what that means 
	RAM[rd] = RAM[rs1] >> RAM[rs2];
}

void slt(uint32_t rd, uint32_t rs1, uint32_t rs2) {
	RAM[rd] = (RAM[rs1] < RAM[rs2]) ? 1 : 0;
}

void sltu(uint32_t rd, uint32_t rs1, uint32_t rs2) { //theres a note of zero-extends. Dont know 
	RAM[rd] = (RAM[rs1] < RAM[rs2]) ? 1 : 0;
}