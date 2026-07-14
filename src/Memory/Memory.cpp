#include "Memory.h"
#include "../CPU/Instructions/R-type/R-Instructions.h"
#include "../CPU/Instructions/I-type/I-Instructions.h"
#include "../CPU/Instructions/S-type/S-Instructions.h"
#include "../CPU/Instructions/B,U,J-type/B,J,U-instructions.h"
#include <string>
#include <iostream>

uint32_t Memory::translate(uint32_t addr)
{
    if (addr < RAM_BASE ||
        addr >= RAM_BASE + memory_size)
    {
        throw 0x100;
    }

    return addr - RAM_BASE;
}

void Memory::write8(uint8_t val, uint32_t addr) {
    RAM[translate(addr)] = val;
}

void Memory::write16(uint16_t val, uint32_t addr) {
    memcpy(&RAM[translate(addr)], &val, sizeof(val));
}
void Memory::write32(uint32_t val, uint32_t addr) {
    memcpy(&RAM[translate(addr)], &val, sizeof(val));
}

uint8_t Memory::read8(uint32_t addr) {
    return RAM[translate(addr)];
}

uint16_t Memory::read16(uint32_t addr) {
    uint16_t temp;
    memcpy(&temp, &RAM[translate(addr)], sizeof(temp));
    return temp;
}

uint32_t Memory::read32(uint32_t addr) {
    uint32_t temp;
    memcpy(&temp, &RAM[translate(addr)], sizeof(temp));
    return temp;
}

//constexpr auto out_of_bounds = 0x1;
//constexpr auto miss_aligned_trap = 0x2;

Decoded_instruction decode_ins(uint32_t ins) {
    Decoded_instruction d{};

    
    d.opcode = ins & 0x7F;
    d.rd = (ins >> 7) & 0x1F;
    d.funct3 = (ins >> 12) & 0x7;
    d.rs1 = (ins >> 15) & 0x1F;
    d.rs2 = (ins >> 20) & 0x1F;
    d.funct7 = (ins >> 25) & 0x7F;
    d.b30 = (ins >> 30) & 0x1;
    d.csr = (ins >> 20) & 0xFFF;
    d.full = ins;

    switch (d.opcode)
    {
        // ---------------- R-type ----------------
    case 0x33:
        d.type = Instruction_type::R;
        d.imm = 0;
        break;

        // ---------------- I-type ----------------
    case 0x03: // loads
    case 0x13: // immediate ALU
    case 0x67: // JALR
    {
        d.type = Instruction_type::I;

        int32_t imm = (ins >> 20) & 0xFFF;

        // 2. Explicitly sign-extend if bit 11 of the immediate (bit 31 of the instruction) is 1
        if (imm & 0x800) {
            imm |= 0xFFFFF000;
        }

        d.imm = imm;

        
        break;
    }

    // ---------------- S-type (store) ----------------
    case 0x23:
    {
        d.type = Instruction_type::S;

        int32_t imm =
            ((ins >> 7) & 0x1F) |
            ((ins >> 25) & 0x7F) << 5;

        // sign-extend 12-bit
        if (imm & 0x800)
            imm |= 0xFFFFF000;

        d.imm = imm;
        break;
    }

    // ---------------- B-type (branch) ----------------
    case 0x63:
    {
        d.type = Instruction_type::B;

        int32_t imm =
            ((ins >> 8) & 0x0F) << 1 |  // bits 4:1
            ((ins >> 25) & 0x3F) << 5 |  // bits 10:5
            ((ins >> 7) & 0x01) << 11 |  // bit 11
            ((ins >> 31) & 0x01) << 12;   // bit 12 (sign)

        // sign-extend 13-bit
        if (imm & 0x1000)
            imm |= 0xFFFFE000;

        d.imm = imm;
        break;
    }

    // ---------------- U-type ----------------
    case 0x37: // LUI
    case 0x17: // AUIPC
    {
        d.type = Instruction_type::U;
        d.imm = (int32_t)(ins & 0xFFFFF000);
        break;
    }

    // ---------------- J-type ----------------
    case 0x6F:
    {
        d.type = Instruction_type::J;

        int32_t imm =
            ((ins >> 21) & 0x3FF) << 1 |  // bits 10:1
            ((ins >> 20) & 0x1) << 11 |  // bit 11
            ((ins >> 12) & 0xFF) << 12 |  // bits 19:12
            ((ins >> 31) & 0x1) << 20;   // sign bit

        // sign-extend 21-bit
        if (imm & 0x100000)
            imm |= 0xFFE00000;

        d.imm = imm;
        break;
    }

    default:
        d.type = Instruction_type::R;
        d.imm = 0;
        break;
    }

    return d;
}

constexpr uint32_t make_key(uint32_t funct7, uint32_t funct3, uint32_t opcode)
{
    uint32_t cls = 0;

    if (opcode == 0x33) { //only r type 
        if (funct7 == 0x20)
            cls = 1;       // SUB/SRA
        else if (funct7 == 0x01)
            cls = 2;       // M extension
    }

    return ((cls & 0x3) << 10) |
        ((funct3 & 0x7) << 7) |
        (opcode & 0x7F);
}


InstructionFunc Instructions[4096] = { nullptr }; 


void Memory::init_table() {
    Instructions[make_key(0, 0, 0x03)] = lb;
    Instructions[make_key(0, 1, 0x03)] = lh;
    Instructions[make_key(0, 2, 0x03)] = lw;
    Instructions[make_key(0, 4, 0x03)] = lbu;
    Instructions[make_key(0, 5, 0x03)] = lhu;

    // --- STORE (Opcode 0x23) ---
    Instructions[make_key(0, 0, 0x23)] = sb;
    Instructions[make_key(0, 1, 0x23)] = sh;
    Instructions[make_key(0, 2, 0x23)] = sw;

    // --- INTEGER REG-IMM (Opcode 0x13) ---
    Instructions[make_key(0, 0, 0x13)] = addi;
    Instructions[make_key(0, 2, 0x13)] = slti;
    Instructions[make_key(0, 3, 0x13)] = sltiu;
    Instructions[make_key(0, 4, 0x13)] = xori;
    Instructions[make_key(0, 6, 0x13)] = ori;
    Instructions[make_key(0, 7, 0x13)] = andi;
    Instructions[make_key(0, 1, 0x13)] = slli;
    Instructions[make_key(0, 5, 0x13)] = srli;
    Instructions[make_key(0x20, 5, 0x13)] = srai;


    // --- INTEGER REG-REG (Opcode 0x33) ---
    Instructions[make_key(0x00, 0, 0x33)] = add;
    Instructions[make_key(0x20, 0, 0x33)] = sub;
    Instructions[make_key(0x00, 5, 0x33)] = srl;
    Instructions[make_key(0x20, 5, 0x33)] = sra;
    Instructions[make_key(0x00, 1, 0x33)] = sll;
    Instructions[make_key(0x00, 2, 0x33)] = slt;
    Instructions[make_key(0x00, 3, 0x33)] = sltu;
    Instructions[make_key(0x00, 4, 0x33)] = _xor;
    Instructions[make_key(0x00, 6, 0x33)] = _or;
    Instructions[make_key(0x00, 7, 0x33)] = _and;

    // --- BRANCH (Opcode 0x63) ---
    Instructions[make_key(0, 0, 0x63)] = beq;
    Instructions[make_key(0, 1, 0x63)] = bne;
    Instructions[make_key(0, 4, 0x63)] = blt;
    Instructions[make_key(0, 5, 0x63)] = bge;
    Instructions[make_key(0, 6, 0x63)] = bltu;
    Instructions[make_key(0, 7, 0x63)] = bgeu;

    // --- JUMP & UPPER IMM (Various Opcodes) ---
    Instructions[make_key(0, 0, 0x6F)] = jal;   // Opcode 0x6F
    Instructions[make_key(0, 0, 0x67)] = jalr;  // Opcode 0x67 (funct3=0)
    Instructions[make_key(0, 0, 0x37)] = lui;   // Opcode 0x37
    Instructions[make_key(0, 0, 0x17)] = auipc; // Opcode 0x17

    Instructions[make_key(0x01, 0, 0x33)] = mul;
    Instructions[make_key(0x01, 1, 0x33)] = mulh;
    Instructions[make_key(0x01, 2, 0x33)] = mulhsu;
    Instructions[make_key(0x01, 3, 0x33)] = mulhu;
    Instructions[make_key(0x01, 4, 0x33)] = div;
    Instructions[make_key(0x01, 5, 0x33)] = divu;
    Instructions[make_key(0x01, 6, 0x33)] = rem;
    Instructions[make_key(0x01, 7, 0x33)] = remu;

    // --- SYSTEM (Opcode 0x73) ---
    Instructions[make_key(0, 1, 0x73)] = csrrw;
    Instructions[make_key(0, 2, 0x73)] = csrrs;
    Instructions[make_key(0, 3, 0x73)] = csrrc;
    Instructions[make_key(0, 5, 0x73)] = csrrwi;
    Instructions[make_key(0, 6, 0x73)] = csrrsi;
    Instructions[make_key(0, 7, 0x73)] = csrrci;
    Instructions[make_key(0, 0, 0x73)] = system;


    instruction_debug_table = {
        // --- LOAD (Opcode 0x03) ---
        { make_key(0x00, 0, 0x03), "lb" },
        { make_key(0x00, 1, 0x03), "lh" },
        { make_key(0x00, 2, 0x03), "lw" },
        { make_key(0x00, 4, 0x03), "lbu" },
        { make_key(0x00, 5, 0x03), "lhu" },

        // --- STORE (Opcode 0x23) ---
        { make_key(0x00, 0, 0x23), "sb" },
        { make_key(0x00, 1, 0x23), "sh" },
        { make_key(0x00, 2, 0x23), "sw" },

        // --- INTEGER REG-IMM (Opcode 0x13) ---
        { make_key(0x00, 0, 0x13), "addi" },
        { make_key(0x00, 2, 0x13), "slti" },
        { make_key(0x00, 3, 0x13), "sltiu" },
        { make_key(0x00, 4, 0x13), "xori" },
        { make_key(0x00, 6, 0x13), "ori" },
        { make_key(0x00, 7, 0x13), "andi" },

        { make_key(0x00, 1, 0x13), "slli" },
        { make_key(0x00, 5, 0x13), "srli" },
        { make_key(0x20, 5, 0x13), "srai" },

        // --- INTEGER REG-REG (Opcode 0x33) ---
        { make_key(0x00, 0, 0x33), "add" },
        { make_key(0x20, 0, 0x33), "sub" },

        { make_key(0x00, 1, 0x33), "sll" },
        { make_key(0x00, 2, 0x33), "slt" },
        { make_key(0x00, 3, 0x33), "sltu" },
        { make_key(0x00, 4, 0x33), "xor" },

        { make_key(0x00, 5, 0x33), "srl" },
        { make_key(0x20, 5, 0x33), "sra" },

        { make_key(0x00, 6, 0x33), "or" },
        { make_key(0x00, 7, 0x33), "and" },

        // --- M EXTENSION (Opcode 0x33, funct7 = 0x01) ---
        { make_key(0x01, 0, 0x33), "mul" },
        { make_key(0x01, 1, 0x33), "mulh" },
        { make_key(0x01, 2, 0x33), "mulhsu" },
        { make_key(0x01, 3, 0x33), "mulhu" },
        { make_key(0x01, 4, 0x33), "div" },
        { make_key(0x01, 5, 0x33), "divu" },
        { make_key(0x01, 6, 0x33), "rem" },
        { make_key(0x01, 7, 0x33), "remu" },

        // --- BRANCH (Opcode 0x63) ---
        { make_key(0x00, 0, 0x63), "beq" },
        { make_key(0x00, 1, 0x63), "bne" },
        { make_key(0x00, 4, 0x63), "blt" },
        { make_key(0x00, 5, 0x63), "bge" },
        { make_key(0x00, 6, 0x63), "bltu" },
        { make_key(0x00, 7, 0x63), "bgeu" },

        // --- JUMP & UPPER IMM ---
        { make_key(0x00, 0, 0x6F), "jal" },
        { make_key(0x00, 0, 0x67), "jalr" },
        { make_key(0x00, 0, 0x37), "lui" },
        { make_key(0x00, 0, 0x17), "auipc" },

        // --- SYSTEM ---
        { make_key(0x00, 0, 0x73), "ecall" }
    };
}

void Memory::init_RAM() { //needs to move
    std::fill(RAM, RAM + memory_size, 0);
}