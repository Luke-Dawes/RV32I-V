#include "../src/Testing/Testing.h"
#include "../src/CPU/Memory.h"
#include "../src/CPU/CPU.h"
#include "../src/assembler/Assembler.h"


int main() {
	CPU cpu;
	init_RAM();
	//run_tests(cpu);

	std::string test_program = 
        R"(# --- SETUP INITIAL VARIABLES ---
# Loop counter (i = 0)
addi x5, x0, 0
# Loop limit (max = 10)
addi x6, x0, 10
# Increment step
addi x7, x0, 1
# Base pointer for memory tests
lui x1, 131072
# Dynamic PC anchor
auipc x2, 0

# --- TYPICAL FORWARD JUMP TEST ---
# Tests if your assembler can handle a forward label before it is defined
jal x3, forward_target

# --- BACKWARD ENTRY POINT (THE MAIN LOOP) ---
loop_start:
    # 1. Register-Immediate Math inside loop
    # Increment loop counter
    addi x5, x5, 1
    # Check if i < 5
    slti x8, x5, 5
    # Check bounds unsigned
    sltiu x9, x5, 12
    # Invert bits
    xori x10, x8, -1
    # Mask bits
    ori x11, x10, 15
    # Filter bits
    andi x12, x11, 3
    # Dynamic offset (i * 4)
    slli x13, x5, 2
    # Logical shift right
    srli x14, x13, 1
    # Arithmetic shift right
    srai x15, x14, 1

    # 2. Memory Ops using loop offset variables
    # Calculate dynamic memory address (base + offset)
    add x16, x1, x13
    # Store byte
    sb x5, 0(x16)
    # Store halfword
    sh x5, 2(x16)
    # Store word
    sw x5, 4(x16)
    # Load byte
    lb x17, 0(x16)
    # Load halfword
    lh x18, 2(x16)
    # Load word
    lw x19, 4(x16)
    # Load byte unsigned
    lbu x20, 0(x16)
    # Load halfword unsigned
    lhu x21, 2(x16)

    # 3. Register-Register Math inside loop
    # Math on loaded loop values
    add x22, x17, x18
    sub x23, x19, x20
    # Shift by variable loop counter
    sll x24, x22, x5
    slt x25, x23, x22
    sltu x26, x24, x25
    xor x27, x25, x26
    # Shift right logical by loop counter
    srl x28, x27, x5
    # Shift right arithmetic by loop counter
    sra x29, x28, x5
    or x30, x29, x30
    and x31, x30, x21

# --- THE LOOP CONDITION CHECKS ---
loop_check:
    # Tests a backward branch (Conditional loop jump)
    blt x5, x6, loop_start

# --- FORWARD TARGET RESOLUTION ---
forward_target:
    # Tests branch equality and inequality jumps
    beq x5, x0, skip_cleanup
    bne x5, x6, complex_branch_test

skip_cleanup:
    # Safe filler math to test spacing
    addi x0, x0, 0

complex_branch_test:
    # Testing remaining unsigned loop/branch boundaries
    bge x6, x5, test_unsigned_branches

test_unsigned_branches:
    bltu x5, x6, test_backward_bgeu

# --- EXTENDED BACKWARD RESOLUTION ---
test_backward_bgeu:
    # Tests a multi-line backward unsigned jump to a previously resolved block
    bgeu x6, x5, loop_check

# --- REG-JUMP AND SYSTEM EXIT ---
exit_sequence:
    # Preps a function address dynamically to test register targeting
    # Jump relative to our anchor x2
    jalr x4, 8(x2)

    # Exit sequence (Will be jumped over by JALR if offsets are correct)
    addi x17, x0, 93
    addi x10, x0, 0
    ecall
)";



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