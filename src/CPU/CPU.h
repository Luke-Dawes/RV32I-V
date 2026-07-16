#pragma once
#include <cstdint>
#include "CSR.h"
#include <optional>
#include "Trap.h"

class Memory;
enum Privilage_mode;

//uint8_t RAM[];

constexpr auto out_of_bounds = 0x1;
constexpr auto miss_aligned_trap = 0x2;

struct Decoded_instruction;

constexpr size_t REG_COUNT = 32;

struct fetched_result {
	uint32_t instruction;
	std::optional<Trap> trap;
};

class CPU {
public:
	CPU(Memory& mem);

	void tick();
	void enter_trap(const Trap& trap);

	bool branch_happended;
	uint32_t registers[REG_COUNT] = { 0 };
	uint32_t PC = 0;
	uint32_t CIR = 0;
	//uint8_t* memory = &RAM[0]; //probably unused as RAM will be global

	Memory& memory;
	CSR csrs;
	Privilage_mode mode;

	std::optional<Trap> load8(uint32_t address, uint8_t& value);
	std::optional<Trap> load16(uint32_t address, uint16_t& value);
	std::optional<Trap> load32(uint32_t address, uint32_t& value);

	std::optional<Trap> store8(uint32_t address, uint8_t value);
	std::optional<Trap> store16(uint32_t address, uint16_t value);
	std::optional<Trap> store32(uint32_t address, uint32_t value);

private:

	fetched_result fetch();
	//decode is in the memory file 
	Decoded_instruction decode(uint32_t ins);
	std::optional<Trap> execute(const Decoded_instruction ins); 
};

