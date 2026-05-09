#pragma once

#include <cstdint>

void sb(uint32_t lower_imm, uint32_t rs1, uint32_t rs2, uint16_t upper_imm);
void sh(uint32_t lower_imm, uint32_t rs1, uint32_t rs2, uint16_t upper_imm);
void sw(uint32_t lower_imm, uint32_t rs1, uint32_t rs2, uint16_t upper_imm);
