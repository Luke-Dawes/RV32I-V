#pragma once
#include <cstdint>
#include <vector>

struct Breakpoint
{
	uint32_t address;
	bool enabled;
};

class CPU;

class Debugger {
public:
	Debugger() = default;

	void step(CPU& cpu);
	void continue_execution(CPU& cpu);

	void dump_mem(CPU& cpu, uint32_t address, uint32_t length);
	void dump_reg(CPU& cpu);

	void add_breakpoint(uint32_t addr);
	void remove_breakpoint(uint32_t addr);
	void toggle_breakpoint(uint32_t addr);

	void shell(CPU& cpu);

private:
	bool has_breakpoint(uint32_t pc);

	std::vector<Breakpoint> breakpoints;

};
