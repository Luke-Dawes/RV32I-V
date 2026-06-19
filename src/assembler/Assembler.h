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
    std::vector<std::string> text;
    

public:

    std::vector<uint32_t> parse();
    std::vector<uint32_t> parse(std::string t) {
        update_string(t);
        return parse();
    }

    Assembler() = default;

    Assembler(std::string t) {
        current = t;
    }

    void update_string(std::string t) {
        current = t;
    }
private:
    void parse_into_vector();
    void first_pass();
    int curr_PC = 0;
    std::unordered_map<std::string, int> symbol_table;
};

enum class Format {
    R_TYPE,
    I_TYPE,
    S_TYPE,
    B_TYPE,
    U_TYPE,
    J_TYPE,
    IS_TYPE,
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

    // === Multiply Extension (RV32M) - Register-Register (R-Type) ===
    {"mul",    {0x02000033, Format::R_TYPE}},
    {"mulh",   {0x02001033, Format::R_TYPE}},
    {"mulhsu", {0x02002033, Format::R_TYPE}},
    {"mulhu",  {0x02003033, Format::R_TYPE}},

    {"div",    {0x02004033, Format::R_TYPE}},
    {"divu",   {0x02005033, Format::R_TYPE}},
    {"rem",    {0x02006033, Format::R_TYPE}},
    {"remu",   {0x02007033, Format::R_TYPE}},

    // === Control Transfer Instructions ===
    {"jal",   {0x0000006F, Format::J_TYPE}},
    {"jalr",  {0x00000067, Format::IS_TYPE}},
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
    {"lb",    {0x00000003, Format::IS_TYPE}},
    {"lh",    {0x00001003, Format::IS_TYPE}},
    {"lw",    {0x00002003, Format::IS_TYPE}},
    {"lbu",   {0x00004003, Format::IS_TYPE}},
    {"lhu",   {0x00005003, Format::IS_TYPE}},

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
    // 0: Zero Register
    {"zero", 0}, {"x0", 0}, {"r0", 0}, {"0", 0},

    // 1: Return Address
    {"ra", 1}, {"x1", 1}, {"r1", 1}, {"1", 1},

    // 2: Stack Pointer
    {"sp", 2}, {"x2", 2}, {"r2", 2}, {"2", 2},

    // 3: Global Pointer
    {"gp", 3}, {"x3", 3}, {"r3", 3}, {"3", 3},

    // 4: Thread Pointer
    {"tp", 4}, {"x4", 4}, {"r4", 4}, {"4", 4},

    // 5-7: Temporary Registers (t0 - t2)
    {"t0", 5}, {"x5", 5}, {"r5", 5}, {"5", 5},
    {"t1", 6}, {"x6", 6}, {"r6", 6}, {"6", 6},
    {"t2", 7}, {"x7", 7}, {"r7", 7}, {"7", 7},

    // 8: Saved Register / Frame Pointer
    {"s0", 8}, {"fp", 8}, {"x8", 8}, {"r8", 8}, {"8", 8},

    // 9: Saved Register
    {"s1", 9}, {"x9", 9}, {"r9", 9}, {"9", 9},

    // 10-17: Function Arguments / Return Values (a0 - a7)
    {"a0", 10}, {"x10", 10}, {"r10", 10}, {"10", 10},
    {"a1", 11}, {"x11", 11}, {"r11", 11}, {"11", 11},
    {"a2", 12}, {"x12", 12}, {"r12", 12}, {"12", 12},
    {"a3", 13}, {"x13", 13}, {"r13", 13}, {"13", 13},
    {"a4", 14}, {"x14", 14}, {"r14", 14}, {"14", 14},
    {"a5", 15}, {"x15", 15}, {"r15", 15}, {"15", 15},
    {"a6", 16}, {"x16", 16}, {"r16", 16}, {"16", 16},
    {"a7", 17}, {"x17", 17}, {"r17", 17}, {"17", 17},

    // 18-27: Saved Registers (s2 - s11)
    {"s2", 18}, {"x18", 18}, {"r18", 18}, {"18", 18},
    {"s3", 19}, {"x19", 19}, {"r19", 19}, {"19", 19},
    {"s4", 20}, {"x20", 20}, {"r20", 20}, {"20", 20},
    {"s5", 21}, {"x21", 21}, {"r21", 21}, {"21", 21},
    {"s6", 22}, {"x22", 22}, {"r22", 22}, {"22", 22},
    {"s7", 23}, {"x23", 23}, {"r23", 23}, {"23", 23},
    {"s8", 24}, {"x24", 24}, {"r24", 24}, {"24", 24},
    {"s9", 25}, {"x25", 25}, {"r25", 25}, {"25", 25},
    {"s10", 26}, {"x26", 26}, {"r26", 26}, {"26", 26},
    {"s11", 27}, {"x27", 27}, {"r27", 27}, {"27", 27},

    // 28-31: Temporary Registers (t3 - t6)
    {"t3", 28}, {"x28", 28}, {"r28", 28}, {"28", 28},
    {"t4", 29}, {"x29", 29}, {"r29", 29}, {"29", 29},
    {"t5", 30}, {"x30", 30}, {"r30", 30}, {"30", 30},
    {"t6", 31}, {"x31", 31}, {"r31", 31}, {"31", 31}
};