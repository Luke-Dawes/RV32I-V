#pragma once
#include <cstdint>

void add(uint32_t rd, uint32_t rs1, uint32_t rs2);
void sub(uint32_t rd, uint32_t rs1, uint32_t rs2);
void _xor(uint32_t rd, uint32_t rs1, uint32_t rs2);
void _or(uint32_t rd, uint32_t rs1, uint32_t rs2);
void _and(uint32_t rd, uint32_t rs1, uint32_t rs2);
void sll(uint32_t rd, uint32_t rs1, uint32_t rs2);
void srl(uint32_t rd, uint32_t rs1, uint32_t rs2);
void sra(uint32_t rd, uint32_t rs1, uint32_t rs2);
void slt(uint32_t rd, uint32_t rs1, uint32_t rs2);
void sltu(uint32_t rd, uint32_t rs1, uint32_t rs2);