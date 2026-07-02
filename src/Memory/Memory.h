#pragma once
#include <cstdint>
#include <unordered_map>
#include <string>
//#include "CPU.h"

class CPU;

using std::uint32_t;


class Memory {

public:

    uint8_t* data() {
        return RAM;
    }

    void write8(uint8_t val, uint32_t addr);
    void write16(uint16_t val, uint32_t addr);
    void write32(uint32_t val, uint32_t addr);

    uint8_t read8(uint32_t addr);
    uint16_t read16(uint32_t addr);
    uint32_t read32(uint32_t addr);

    Memory() {
        RAM = new uint8_t[1024 * 1024](); //1MB
        init_table();
        init_RAM();
    }

    ~Memory() {
        delete[] RAM;
    }

    static constexpr uint32_t memory_size = 1024 * 1024;
    uint32_t RAM_BASE = 0x80000000;

private:

    void init_table();

    void init_RAM();

    //little edian memory 
    uint8_t* RAM;
    

};


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





typedef void (*InstructionFunc)(CPU& cpu, Decoded_instruction& ins);

extern InstructionFunc Instructions[];
inline std::unordered_map<uint32_t, std::string> instruction_debug_table;

constexpr uint32_t make_key(uint32_t funct7, uint32_t funct3, uint32_t opcode);