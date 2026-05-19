#pragma once
#include <cstdint>

//uint8_t RAM[];

constexpr auto out_of_bounds = 0x1;
constexpr auto miss_aligned_trap = 0x2;

struct Decoded_instruction;

constexpr size_t REG_COUNT = 32;

enum stage {
	FETCH, DECODE, EXECUTE
};


class CPU {
public:
	CPU();

	void tick();

	stage current_stage;
	bool branch_happended;
	uint32_t registers[REG_COUNT] = { 0 };
	uint32_t PC = 0;
	uint32_t CIR = 0;
	//uint8_t* memory = &RAM[0]; //probably unused as RAM will be global

private:
	uint32_t fetch();
	//decode is in the memory file 
	Decoded_instruction decode(uint32_t ins);
	void execute(const Decoded_instruction ins); //idk where im storing this 
};