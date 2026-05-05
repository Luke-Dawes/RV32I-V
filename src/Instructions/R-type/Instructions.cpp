#include "Instructions.h"
#include "../../CPU/Memory.h"
#include "../../CPU/Memory.h" //only way intellisense will work is include it twice


void add(uint32_t rs, uint32_t rs1, uint32_t rs2) {
	RAM[rs] = RAM[rs1] + RAM[rs2];
}

void sub(uint32_t rs, uint32_t rs1, uint32_t rs2) {
	RAM[rs] = RAM[rs1] - RAM[rs2];
}

void XOR(uint32_t rs, uint32_t rs1, uint32_t rs2) {
	RAM[rs] = RAM[rs1] ^ RAM[rs2];
}

