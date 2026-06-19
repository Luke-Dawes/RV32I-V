# RV32I

simple attempt at simulating a RISC-V (RV32I)

https://docs.riscv.org/reference/isa/unpriv/rv32.html - documentation

https://msyksphinz-self.github.io/riscv-isadoc/html/rvi.html - full link of all functions

https://dejazzer.com/coen2710/lectures/RISC-V-Reference-Data-Green-Card.pdf - cheat sheet

https://www.cs.sfu.ca/~ashriram/Courses/CS295/assets/notebooks/RISCV/RISCV_CARD.pdf - another cheat sheet

https://docs.riscv.org/reference/isa/v20260120/unpriv/m-st-ext.html m extention



# long term aim

build this, with the base 40 instructions - done

build a like interpreter which translates RISC-V assembly into binary codes - done

## left to do

add the M extension - multiplication - currently working on, just need to create testing for it

elf loader? and debugger - potentials

add csr - (control and status registers - exceptions and interrupts)

simple cache

virtual memory

scheduler (need privilge modes?)

restructure files - i.e. testing should be outside src - there should be multiple cpp files for the single testing header file which are named like testing_cpu, testing_instructions etc, instructions in CPU, memory in its own folder 

system calls

memory management

performance manager? - i.e. tracks amount of instructions done, cache misses etc
