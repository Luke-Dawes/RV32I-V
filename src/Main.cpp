#include "../Testing/Testing.h"
#include "../src/Memory/Memory.h"
#include "../src/CPU/CPU.h"
#include "../src/assembler/Assembler.h"
#include "../src/Elf_loader/Loader.h"
#include "../src/debugger/Debugger.h"
#include <fstream>


int main() {
	Memory mem;
	CPU cpu(mem);
	//run_cpu_tests(cpu); tests are currently broken due to me changing ram to start at 0x8000... compared to 0.
    //run_instruction_tests(cpu);
	//run_assembler_tests(cpu);

	Elf_Loader load;

	std::fstream file("hidden.txt");

	std::string loc;

	std::getline(file, loc);

	load.load(cpu, loc);

	Debugger debug;

	//debug.shell(cpu); user CLI interphase 

	//debug.continue_execution(cpu);

	return 1;
}