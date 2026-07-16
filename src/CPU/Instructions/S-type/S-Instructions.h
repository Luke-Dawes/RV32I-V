#pragma once
#include <cstdint>
#include <optional>

class CPU;
struct Decoded_instruction;
struct Trap;

std::optional<Trap> sb(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> sh(CPU& cpu, const Decoded_instruction& ins);
std::optional<Trap> sw(CPU& cpu, const Decoded_instruction& ins);
