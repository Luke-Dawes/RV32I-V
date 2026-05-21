#pragma once
#include <unordered_map>
#include <string>

/*  
	* lexical analysis 
	* find a full word - i.e. find the next space
	* 
	* parser
	* go from addi to that opcode - done using a simple map 
	* 
	* find the parameters i.e. what addi takes in (rd/rs1/rs2/imm etc)
	* this can be a simple switch case statement based on format
	* 
	* 
	* 
*/

class Assembler {

    std::string current;

    

public:

    std::vector<uint32_t> parse();

    Assembler() = default;

    Assembler(std::string t) {
        current = t;
    }

    void update_string(std::string t) {
        current = t;
    }

};

enum class Format {
    R_TYPE,
    I_TYPE,
    S_TYPE,
    B_TYPE,
    U_TYPE,
    J_TYPE,
    SYSTEM // For ecall/ebreak which have fixed configurations
};

struct EncodingInfo {
    uint32_t base_mask;
    Format format;
};

const std::unordered_map<std::string, EncodingInfo> encoding = {
    // === Integer Computational Instructions - Immediate (I-Type) ===
    {"addi",  {0x00000013, Format::I_TYPE}},
    {"slti",  {0x00002013, Format::I_TYPE}},
    {"sltiu", {0x00003013, Format::I_TYPE}},
    {"andi",  {0x00007013, Format::I_TYPE}},
    {"ori",   {0x00006013, Format::I_TYPE}},
    {"xori",  {0x00004013, Format::I_TYPE}},
    {"slli",  {0x00001013, Format::I_TYPE}}, // Note: imm[11:5] must be 0x00
    {"srli",  {0x00005013, Format::I_TYPE}}, // Note: imm[11:5] must be 0x00
    {"srai",  {0x40005013, Format::I_TYPE}}, // Note: imm[11:5] must be 0x20

    // === Integer Computational Instructions - Register-Register (R-Type) ===
    {"add",   {0x00000033, Format::R_TYPE}},
    {"sub",   {0x40000033, Format::R_TYPE}},
    {"sll",   {0x00001033, Format::R_TYPE}},
    {"slt",   {0x00002033, Format::R_TYPE}},
    {"sltu",  {0x00003033, Format::R_TYPE}},
    {"xor",   {0x00004033, Format::R_TYPE}},
    {"srl",   {0x00005033, Format::R_TYPE}},
    {"sra",   {0x40005033, Format::R_TYPE}},
    {"or",    {0x00006033, Format::R_TYPE}},
    {"and",   {0x00007033, Format::R_TYPE}},

    // === Control Transfer Instructions ===
    {"jal",   {0x0000006F, Format::J_TYPE}},
    {"jalr",  {0x00000067, Format::I_TYPE}},
    {"beq",   {0x00000063, Format::B_TYPE}},
    {"bne",   {0x00001063, Format::B_TYPE}},
    {"blt",   {0x00004063, Format::B_TYPE}},
    {"bge",   {0x00005063, Format::B_TYPE}},
    {"bltu",  {0x00006063, Format::B_TYPE}},
    {"bgeu",  {0x00007063, Format::B_TYPE}},

    // === Upper Immediate Instructions (U-Type) ===
    {"lui",   {0x00000037, Format::U_TYPE}},
    {"auipc", {0x00000017, Format::U_TYPE}},

    // === Load Instructions (I-Type) ===
    {"lb",    {0x00000003, Format::I_TYPE}},
    {"lh",    {0x00001003, Format::I_TYPE}},
    {"lw",    {0x00002003, Format::I_TYPE}},
    {"lbu",   {0x00004003, Format::I_TYPE}},
    {"lhu",   {0x00005003, Format::I_TYPE}},

    // === Store Instructions (S-Type) ===
    {"sb",    {0x00000023, Format::S_TYPE}},
    {"sh",    {0x00001023, Format::S_TYPE}},
    {"sw",    {0x00002023, Format::S_TYPE}},

    // === Memory Ordering and System Instructions ===
    {"fence", {0x0000000F, Format::I_TYPE}},
    {"ecall", {0x00000073, Format::SYSTEM}},
    {"ebreak",{0x00100073, Format::SYSTEM}}
};

const std::unordered_map<std::string, uint32_t> register_to_number = {
    {"r1", 1},
    {"1", 1},
    {"x1", 1},
    {"r2", 2},
    {"2", 2},
    {"x2", 2}

};