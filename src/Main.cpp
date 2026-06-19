#include "../Testing/Testing.h"
#include "../src/Memory/Memory.h"
#include "../src/CPU/CPU.h"
#include "../src/assembler/Assembler.h"


int main() {
	CPU cpu;
	init_RAM();
	run_cpu_tests(cpu);
    run_instruction_tests(cpu);
	run_assembler_tests(cpu);

	return 1;
}