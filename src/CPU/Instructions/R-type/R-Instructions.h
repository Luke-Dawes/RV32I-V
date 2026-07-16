#pragma once
#include <cstdint>
#include <optional>

class CPU;
struct Decoded_instruction;
struct Trap;

std::optional<Trap> add(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> sub(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> _xor(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> _or(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> _and(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> sll(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> srl(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> sra(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> slt(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> sltu(CPU& cpu, const Decoded_instruction& ins);

std::optional<Trap> mul(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> mulh(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> mulhsu(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> mulhu(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> div(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> divu(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> rem(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> remu(CPU& cpu, const Decoded_instruction& ins);
