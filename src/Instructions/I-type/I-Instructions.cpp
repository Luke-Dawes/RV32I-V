#include "I-Instructions.h"
#include "../../CPU/Memory.h"

inline int32_t sixteen_to_thirtytwo(uint16_t imm) {
	return (static_cast<int32_t>(imm) << 20) >> 20;
}

void addi(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) {
	if (rd != 0) registers[rd] = registers[rs1] + sixteen_to_thirtytwo(imm);
}

void xori(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) {
	if (rd != 0) registers[rd] = registers[rs1] ^ sixteen_to_thirtytwo(imm);
}

void ori(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) {
	if (rd != 0) registers[rd] = registers[rs1] | sixteen_to_thirtytwo(imm);
}

void andi(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) {
	if (rd != 0) registers[rd] = registers[rs1] & sixteen_to_thirtytwo(imm);
}

void slli(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) {
	if (rd != 0) registers[rd] = registers[rs1] << (imm & 0x1F);
}

void srli(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) {
	if (rd != 0) registers[rd] = registers[rs1] >> (imm & 0x1F);
}

void srai(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) { //msb extends - signed
	if (rd != 0) registers[rd] = static_cast<int32_t>(registers[rs1]) >> (imm & 0x1F);
}

void slti(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) { //signed
	if (rd != 0) registers[rd] = (static_cast<int32_t>(registers[rs1]) < imm) ? 1 : 0;
}

void sltiu(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) { //unsigned
	if (rd != 0) registers[rd] = (registers[rs1] < static_cast<uint32_t>(imm)) ? 1 : 0;
}


void lb(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) {
	if (rd != 0) {
		registers[rd] = static_cast<int32_t>(static_cast<int8_t>(RAM[rs1 + imm]));
	}
}

//signed
void lh(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) {
	if (rd != 0) {
		int32_t val = RAM[rs1 + imm] | (RAM[rs1 + imm + 1] << 8);
		registers[rd] = (val << 16) >> 16;
	}
}
//signed
void l(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) {
	if (rd != 0) {
		registers[rd] = static_cast<int32_t>(
			RAM[rs1 + imm] |
			(RAM[rs1 + imm + 1] << 8) |
			(RAM[rs1 + imm + 2] << 16) |
			(RAM[rs1 + imm + 3] << 24)
			);
	}
}
//unsigned
void lbu(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) {
	if (rd != 0) {
		registers[rd] = static_cast<uint32_t>(RAM[rs1 + imm]);
	}
}

void lhu(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm) {
	if (rd != 0) {
		registers[rd] = static_cast<uint32_t>(RAM[rs1 + imm]) | (RAM[rs1 + imm + 1] << 8);
	}
}