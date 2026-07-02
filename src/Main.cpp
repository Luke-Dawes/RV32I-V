#include "../Testing/Testing.h"
#include "../src/Memory/Memory.h"
#include "../src/CPU/CPU.h"
#include "../src/assembler/Assembler.h"


int main() {
	Memory mem;
	CPU cpu(mem);
	run_cpu_tests(cpu);
    run_instruction_tests(cpu);
	run_assembler_tests(cpu);

	return 1;
}