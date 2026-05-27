#include "../src/Testing/Testing.h"
#include "../src/CPU/Memory.h"
#include "../src/CPU/CPU.h"
#include "../src/assembler/Assembler.h"


int main() {
	CPU cpu;
	init_RAM();
	//run_tests(cpu);

	std::string test_program = R"(lui x10, 65536
addi x11, x0, 5
addi x12, x0, 0
lw x13, 0(x10)
addi x14, x0, 42
bne x13, x14, skip
addi x12, x12, 1
sw x12, 4(x10)
addi x10, x10, 4
addi x11, x11, -1
bne x11, x0, loop_start
jal x0, exit)";

	std::string test_program1 =
		"addi x1 x0 5\n"
		"loop_start:\n"
		"addi x1 x1 -1\n"
		"bne x1 x0 loop_start\n"
		"jal x0 loop_start";

	Assembler assembler(test_program);

	assembler.parse();

	return 1;
}