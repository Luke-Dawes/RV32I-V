#pragma once
#include <cstdint>

void add(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
void sub(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
void _xor(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
void _or(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
void _and(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
void sll(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
void srl(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
void sra(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
void slt(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
void sltu(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);