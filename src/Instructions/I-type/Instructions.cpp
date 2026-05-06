#include "Instructions.h"
#include "../../CPU/Memory.h"

inline int32_t sixteen_to_thirtytwo(uint16_t imm) {
	return (static_cast<int32_t>(imm) << 20) >> 20;
}

void addi(uint32_t rd, uint32_t rs1, uint16_t imm) {
	if (rd != 0) registers[rd] = registers[rs1] + sixteen_to_thirtytwo(imm);
}

void xori(uint32_t rd, uint32_t rs1, uint16_t imm) {
	if (rd != 0) registers[rd] = registers[rs1] ^ sixteen_to_thirtytwo(imm);
}

void ori(uint32_t rd, uint32_t rs1, uint16_t imm) {
	if (rd != 0) registers[rd] = registers[rs1] | sixteen_to_thirtytwo(imm);
}

void andi(uint32_t rd, uint32_t rs1, uint16_t imm) {
	if (rd != 0) registers[rd] = registers[rs1] & sixteen_to_thirtytwo(imm);
}

void slli(uint32_t rd, uint32_t rs1, uint16_t imm) {
	if (rd != 0) registers[rd] = registers[rs1] << (imm & 0x1F);
}

void srli(uint32_t rd, uint32_t rs1, uint16_t imm) {
	if (rd != 0) registers[rd] = registers[rs1] >> (imm & 0x1F);
}
void srai(uint32_t rd, uint32_t rs1, uint16_t imm) { //msb extends - signed
	if (rd != 0) registers[rd] = static_cast<int32_t>(registers[rs1]) >> (imm & 0x1F);
}
void slti(uint32_t rd, uint32_t rs1, uint16_t imm) { //signed
	if (rd != 0) registers[rd] = (static_cast<int32_t>(registers[rs1]) < imm) ? 1 : 0;
}
void sltiu(uint32_t rd, uint32_t rs1, uint16_t imm) { //unsigned
	if (rd != 0) registers[rd] = (registers[rs1] < static_cast<uint32_t>(imm)) ? 1 : 0;
}