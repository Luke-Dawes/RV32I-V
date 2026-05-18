#pragma once
#include <cstdint>
//#include "CPU.h"

class CPU;

using std::uint32_t;

//little edian memory 
extern uint8_t RAM[];





enum class Instruction_type {
    R, I, S, B, U, J
};

struct Decoded_instruction {
    Instruction_type type;

    uint8_t rd;
    uint8_t rs1;
    uint8_t rs2;

    int32_t imm;

    uint8_t opcode;
    uint8_t funct3;
    uint8_t funct7;
};

Decoded_instruction decode_ins(uint32_t PC);

void init_table();

void init_RAM();



typedef void (*InstructionFunc)(CPU& cpu, Decoded_instruction& ins);

extern InstructionFunc Instructions[];