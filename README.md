# RV32I

simple attempt at simulating a RISC-V (RV32I)

https://docs.riscv.org/reference/isa/unpriv/rv32.html - documentation

https://msyksphinz-self.github.io/riscv-isadoc/html/rvi.html - full link of all functions

https://dejazzer.com/coen2710/lectures/RISC-V-Reference-Data-Green-Card.pdf - cheat sheet

https://www.cs.sfu.ca/~ashriram/Courses/CS295/assets/notebooks/RISCV/RISCV_CARD.pdf - another cheat sheet



# long term aim

build this, with the base 40 instructions - done

build a like interpreter which translates python or c++ or some sort of psuedocode to these instructions. - partially done - building assembler for assembly.

## left to do

add the M extension - multiplication

elf loader? and debugger - potentials

add csr - (control and status registers - exceptions and interrupts)

simple cache

virtual memory

scheduler (need privilge modes?)

system calls

memory management

performance manager? - i.e. tracks amount of instructions done, cache misses etc
