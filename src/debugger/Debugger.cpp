#include <iostream>
#include <string>
#include <iomanip>
#include "Debugger.h"
#include "../CPU/CPU.h"
#include "../Memory/Memory.h"

void Debugger::step(CPU& cpu) {

	cpu.tick();
	dump_reg(cpu);
	dump_mem(cpu, cpu.PC, 64);

}

void Debugger::dump_mem(CPU& cpu, uint32_t address, uint32_t length) {

	for (uint32_t i = 0; i < length; i++) {

		if (i % 16 == 0) {
			std::cout << "\n" << std::hex << address + i << ": ";
		}

		std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(cpu.memory.read8(address + i)) << ' ';
	} 

}

void Debugger::dump_reg(CPU& cpu) {

    std::cout << "PC: " << std::hex << cpu.PC << "\n";


    for (int i = 0; i < 32; i++)
    {
        std::cout << "x" << i << ": " << cpu.registers[i] << "\n";
    }

}

bool Debugger::has_breakpoint(uint32_t pc) {

	for (auto bp : breakpoints) {
		if (bp.address == pc && bp.enabled) {
			return true;
		}
	}
	return false;
}

void Debugger::add_breakpoint(uint32_t addr) {
	breakpoints.push_back({addr, true});
}

void Debugger::toggle_breakpoint(uint32_t addr) {

	auto it = breakpoints.begin();

	while (it != breakpoints.end()) {
		if (it->address == addr) {
			it->enabled = !it->enabled;
			return;
		}
		++it;
	}

}

void Debugger::remove_breakpoint(uint32_t addr) {
	
	auto it = breakpoints.begin();

	while (it != breakpoints.end()) {
		if (it->address == addr) {
			breakpoints.erase(it);
			return;
		}
		++it;
	}
}

void Debugger::continue_execution(CPU& cpu) {

	if (!has_breakpoint(cpu.PC)) {
		cpu.tick();
		dump_reg(cpu);
		dump_mem(cpu, cpu.PC, 64);
	}

	std::cout << "Breakpoint hit at 0x"
		<< std::hex << cpu.PC << '\n';


}

void Debugger::shell(CPU& cpu) {
	std::string command;

	//have to manually add breakpoints due to std::cin and i cba to change it to getline

	while (true)
	{
		std::cout << "(debug) ";
		std::cin >> command;


		if (command == "step")
		{
			step(cpu);
		}


		else if (command == "continue")
		{
			continue_execution(cpu);
		}


		else if (command == "regs")
		{
			dump_reg(cpu);
		}

		else if (command == "mem") {
			dump_mem(cpu, 0, cpu.memory.memory_size);
		}
	}
}