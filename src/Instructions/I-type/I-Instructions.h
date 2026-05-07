#pragma once
#include <cstdint>


void addi(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
void xori(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
void ori(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
void andi(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
void slli(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
void srli(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
void srai(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
void slti(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
void sltiu(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);


void lb(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
void lh(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
void lw(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
void lbu(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);
void lhu(uint32_t rd, uint32_t rs1, uint32_t rs2, uint16_t imm);