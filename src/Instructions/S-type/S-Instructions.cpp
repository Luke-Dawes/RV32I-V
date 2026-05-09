#include "S-Instructions.h"
#include "../../CPU/Memory.h"

//imm is 00001111111111, where the bottom 5 bits are rs2
//so i need to set the bottom 5 bits to 0

#define S_imm(imm, rd) ((imm & 0xFFE0) | rd)

void sb(uint32_t lower_imm, uint32_t rs1, uint32_t rs2, uint16_t upper_imm) {
	RAM[registers[rs1] + S_imm(upper_imm, lower_imm)] = registers[rs2] & 0xFF;
}

void sh(uint32_t lower_imm, uint32_t rs1, uint32_t rs2, uint16_t upper_imm) {
	RAM[registers[rs1] + S_imm(upper_imm, lower_imm)] = registers[rs2] & 0xFFFF;
}
void sw(uint32_t lower_imm, uint32_t rs1, uint32_t rs2, uint16_t upper_imm) {
	RAM[registers[rs1] + S_imm(upper_imm, lower_imm)] = registers[rs2];
}