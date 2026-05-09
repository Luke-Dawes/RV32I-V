#include "R-Instructions.h"
#include "../../CPU/Memory.h"

void add(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) {
	if (rd != 0) registers[rd] = registers[rs1] + registers[rs2];
}

void sub(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) {
	if (rd != 0) registers[rd] = registers[rs1] - registers[rs2];
}

void _xor(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) {
	if (rd != 0) registers[rd] = registers[rs1] ^ registers[rs2];
}

void _or(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) {
	if (rd != 0) registers[rd] = registers[rs1] | registers[rs2];
}

void _and(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) {
	if (rd != 0) registers[rd] = registers[rs1] & registers[rs2];
}

void sll(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) {
	if (rd != 0) registers[rd] = registers[rs1] << (registers[rs2] & 0x1F);
}

void srl(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) {
	if (rd != 0) registers[rd] = registers[rs1] >> (registers[rs2] & 0x1F);
}

//if the number is negative it the empty slots created by shifting are filled with 1s.
void sra(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) {
	if (rd != 0) registers[rd] = static_cast<int32_t>(registers[rs1]) >> (registers[rs2] & 0x1F);
}

//signed comparison
void slt(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) {
	if (rd != 0) registers[rd] = (static_cast<int32_t>(registers[rs1]) < static_cast<int32_t>(registers[rs2])) ? 1 : 0;
}

//unsigned comparision
void sltu(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) { //theres a note of zero-extends - no sign 
	if (rd != 0) registers[rd] = (registers[rs1] < registers[rs2]) ? 1 : 0;
}