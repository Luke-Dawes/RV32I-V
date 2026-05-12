#include "../src/Testing/Testing.h"
#include "../src/CPU/Memory.h"


int main() {
	CPU cpu;
	init_RAM();
	run_tests(cpu);
	return 1;
}