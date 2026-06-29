#pragma once
#include <cstdint>
#include <unordered_map>
#include <string>
//#include "CPU.h"

class CPU;

using std::uint32_t;

//little edian memory 
extern uint8_t RAM[];
constexpr uint32_t RAM_BASE = 0x80000000;




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
    uint32_t b30;
};

Decoded_instruction decode_ins(uint32_t PC);

void init_table();

void init_RAM();



typedef void (*InstructionFunc)(CPU& cpu, Decoded_instruction& ins);

extern InstructionFunc Instructions[];
inline std::unordered_map<uint32_t, std::string> instruction_debug_table;

constexpr uint32_t make_key(uint32_t funct7, uint32_t funct3, uint32_t opcode);