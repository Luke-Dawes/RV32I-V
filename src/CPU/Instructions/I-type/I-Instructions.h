#pragma once
#include <cstdint>
#include <optional>

class CPU;
struct Decoded_instruction;
struct Trap;



std::optional<Trap> addi(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> xori(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> ori(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> andi(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> slli(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> srli(CPU& cpu, const  Decoded_instruction& ins);
std::optional<Trap> srai(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> slti(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> sltiu(CPU& cpu, const Decoded_instruction& ins);


std::optional<Trap> lb(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> lh(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> lw(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> lbu(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> lhu(CPU& cpu, const Decoded_instruction& ins);

std::optional<Trap> jalr(CPU& cpu, const  Decoded_instruction& ins);

std::optional<Trap> system(CPU& cpu, const Decoded_instruction& ins);

std::optional<Trap> csrrw(CPU& cpu, const  Decoded_instruction& ins);
std::optional<Trap> csrrs(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> csrrc(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> csrrwi(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> csrrsi(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> csrrci(CPU& cpu, const Decoded_instruction& ins);

