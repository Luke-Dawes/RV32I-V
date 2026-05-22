#include "../src/Testing/Testing.h"
#include "../src/CPU/Memory.h"
#include "../src/CPU/CPU.h"
#include "../src/assembler/Assembler.h"


int main() {
	CPU cpu;
	init_RAM();
	//run_tests(cpu);

	Assembler assembler("add x1 x2 x1");

	assembler.parse();

	return 1;
}