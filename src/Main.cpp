#include "../src/Testing/Testing.h"
#include "../src/CPU/Memory.h"
#include "../src/CPU/CPU.h"


int main() {
	CPU cpu;
	init_RAM();
	run_tests(cpu);
	return 1;
}