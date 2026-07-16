#pragma once
#include <optional>


class CPU;
struct Decoded_instruction;
struct Trap;

// ----- B type ----------
std::optional<Trap> beq(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> bne(CPU& cpu, const  Decoded_instruction& ins);
std::optional<Trap> blt(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> bge(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> bltu(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> bgeu(CPU& cpu, const Decoded_instruction& ins);

// ----- J type ----------
std::optional<Trap> jal(CPU& cpu, const Decoded_instruction& ins);

// ----- U type ----------
std::optional<Trap> lui(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> auipc(CPU& cpu, const Decoded_instruction& ins);