#include "testing.h"

#include <cassert>
#include <iostream>
#include "../Assembler/Assembler.h"

void run_assembler_tests(CPU& cpu)
{
    Assembler assembler;
    test_r_type(assembler);
    test_i_type(assembler);
    test_s_type(assembler);
    test_b_type(assembler);
    test_u_type(assembler);
    test_j_type(assembler);

    test_forward_jal(assembler);
    test_backward_branch(assembler);

    test_small_main_program(assembler);
    test_big_main_program(assembler);

    std::cout << "Assembler tests passed\n";
}

void test_b_type(Assembler& assembler)
{
    // Forward 8 bytes
    assert(assembler.parse("beq x1, x2, 8")[0] == 0x00208463);
    assert(assembler.parse("bne x1, x2, 8")[0] == 0x00209463);
    assert(assembler.parse("blt x1, x2, 8")[0] == 0x0020c463);
    assert(assembler.parse("bge x1, x2, 8")[0] == 0x0020d463);
    assert(assembler.parse("bltu x1, x2, 8")[0] == 0x0020e463);
    assert(assembler.parse("bgeu x1, x2, 8")[0] == 0x0020f463);
}

void test_j_type(Assembler& assembler)
{
    assert(assembler.parse("jal x1, 8")[0] == 0x008000ef);
    assert(assembler.parse("jal x3, 120")[0] == 0x078001ef);

    assert(assembler.parse("jal x1, -4")[0] == 0xffdff0ef);
}

void test_b_type_negative(Assembler& assembler)
{
    assert(assembler.parse("beq x1, x2, -4")[0] == 0xfe208ee3);
    assert(assembler.parse("bne x1, x2, -4")[0] == 0xfe209ee3);
    assert(assembler.parse("blt x1, x2, -4")[0] == 0xfe20cee3);
    assert(assembler.parse("bge x1, x2, -4")[0] == 0xfe20dee3);
    assert(assembler.parse("bltu x1, x2, -4")[0] == 0xfe20eee3);
    assert(assembler.parse("bgeu x1, x2, -4")[0] == 0xfe20fee3);
}

void test_i_type(Assembler& assembler)
{
    assert(assembler.parse("addi x5, x0, 0")[0] == 0x00000293);
    assert(assembler.parse("addi x6, x0, 10")[0] == 0x00a00313);

    assert(assembler.parse("slti x8, x5, 5")[0] == 0x0052a413);
    assert(assembler.parse("sltiu x9, x5, 12")[0] == 0x00c2b493);

    assert(assembler.parse("xori x10, x8, -1")[0] == 0xfff44513);
    assert(assembler.parse("ori x11, x10, 15")[0] == 0x00f56593);
    assert(assembler.parse("andi x12, x11, 3")[0] == 0x0035f613);

    assert(assembler.parse("slli x13, x5, 2")[0] == 0x00229693);
    assert(assembler.parse("srli x14, x13, 1")[0] == 0x0016d713);
    assert(assembler.parse("srai x15, x14, 1")[0] == 0x40175793);

    assert(assembler.parse("lb x17, 0(x16)")[0] == 0x00080883);
    assert(assembler.parse("lh x18, 2(x16)")[0] == 0x00281903);
    assert(assembler.parse("lw x19, 4(x16)")[0] == 0x00482983);
    assert(assembler.parse("lbu x20, 0(x16)")[0] == 0x00084a03);
    assert(assembler.parse("lhu x21, 2(x16)")[0] == 0x00285a83);

    assert(assembler.parse("jalr x4, 8(x2)")[0] == 0x00810267);

    assert(assembler.parse("ecall")[0] == 0x00000073);
}

void test_r_type(Assembler& assembler)
{
    assert(assembler.parse("add x16, x1, x13")[0] == 0x00d08833);

    assert(assembler.parse("add x22, x17, x18")[0] == 0x01288b33);
    assert(assembler.parse("sub x23, x19, x20")[0] == 0x41498bb3);

    assert(assembler.parse("sll x24, x22, x5")[0] == 0x005b1c33);

    assert(assembler.parse("slt x25, x23, x22")[0] == 0x016bacb3);
    assert(assembler.parse("sltu x26, x24, x25")[0] == 0x019c3d33);

    assert(assembler.parse("xor x27, x25, x26")[0] == 0x01accdb3);

    assert(assembler.parse("srl x28, x27, x5")[0] == 0x005dde33);
    assert(assembler.parse("sra x29, x28, x5")[0] == 0x405e5eb3);

    assert(assembler.parse("or x30, x29, x30")[0] == 0x01eeef33);
    assert(assembler.parse("and x31, x30, x21")[0] == 0x015f7fb3);
}

void test_s_type(Assembler& assembler)
{
    assert(assembler.parse("sb x5, 0(x16)")[0] == 0x00580023);
    assert(assembler.parse("sh x5, 2(x16)")[0] == 0x00581123);
    assert(assembler.parse("sw x5, 4(x16)")[0] == 0x00582223);
}

void test_u_type(Assembler& assembler)
{
    assert(assembler.parse("lui x1, 131072")[0] == 0x000200b7);
    assert(assembler.parse("auipc x2, 0")[0] == 0x00000117);
}

void test_forward_jal(Assembler& assembler)
{
    const char* source =
        "jal x3, target\n"
        "addi x0, x0, 0\n"
        "target:\n"
        "addi x1, x0, 1\n";

    auto result = assembler.parse(source);

    assert(result.size() == 3);

    assert(result[0] == 0x008001ef);
    assert(result[1] == 0x00000013);
    assert(result[2] == 0x00100093);
}

void test_backward_branch(Assembler& assembler)
{
    const char* source =
        "loop:\n"
        "addi x5, x5, 1\n"
        "blt x5, x6, loop\n";

    auto result = assembler.parse(source);

    assert(result.size() == 2);

    assert(result[0] == 0x00128293);

    // Just verify branch assembled.
    assert(result[1] != 0);
}

void test_small_main_program(Assembler& assembler) {

    std::string test_program =
        "addi x1 x0 5\n"
        "loop_start:\n"
        "addi x1 x1 -1\n"
        "bne x1 x0 loop_start\n"
        "jal x0 loop_start";

    auto result = assembler.parse(test_program);

    assert(result.size() == 4 &&
        "Small program should assemble to 4 instructions");

    assert(result[0] == 0x00500093 &&
        "addi x1,x0,5 incorrect");

    assert(result[1] == 0xfff08093 &&
        "addi x1,x1,-1 incorrect");

    assert(result[2] != 0 &&
        "bne failed to assemble");

    assert(result[3] != 0 &&
        "jal failed to assemble");

}

void test_big_main_program(Assembler& assembler) {
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

    auto result = assembler.parse(test_program);

    assert(result.size() == 45 &&
        "Big test program should produce 46 instructions");

    // Beginning
    assert(result[0] == 0x00000293);
    assert(result[1] == 0x00a00313);
    assert(result[2] == 0x00100393);
    assert(result[3] == 0x000200b7);
    assert(result[4] == 0x00000117);

    // Forward JAL
    assert(result[5] == 0x078001ef &&
        "Forward label JAL incorrect");

    // First load/store section
    assert(result[16] == 0x00580023);
    assert(result[17] == 0x00581123);
    assert(result[18] == 0x00582223);

    // Branches
    assert(result[34] == 0xf862c8e3 &&
        "blt loop branch incorrect");

    assert(result[35] == 0x00028463 &&
        "beq incorrect");

    assert(result[36] == 0x00629463 &&
        "bne incorrect");

    // JALR
    assert(result[41] == 0x00810267 &&
        "jalr incorrect");

    // Exit sequence
    assert(result[42] == 0x05d00893);
    assert(result[43] == 0x00000513);
    assert(result[44] == 0x00000073 &&
        "ecall incorrect");
}