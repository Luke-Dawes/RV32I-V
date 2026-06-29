# RV32I

simple attempt at simulating a RISC-V (RV32I)

https://docs.riscv.org/reference/isa/unpriv/rv32.html - documentation

https://msyksphinz-self.github.io/riscv-isadoc/html/rvi.html - full link of all functions

https://dejazzer.com/coen2710/lectures/RISC-V-Reference-Data-Green-Card.pdf - cheat sheet

https://www.cs.sfu.ca/~ashriram/Courses/CS295/assets/notebooks/RISCV/RISCV_CARD.pdf - another cheat sheet

https://docs.riscv.org/reference/isa/v20260120/unpriv/m-st-ext.html m extention

https://gist.github.com/x0nu11byt3/bcb35c3de461e5fb66173071a2379779 elf loader



# long term aim

build this, with the base 40 instructions - done

build a like interpreter which translates RISC-V assembly into binary codes - done

add the M extension - multiplication - testing not done however

## left to do

elf loader? and debugger - potentials / stack memory etc

add csr - (control and status registers - exceptions and interrupts

simple cache

virtual memory - sv48/sv57

scheduler (need privilge modes?)

system calls

memory management

performance manager? - i.e. tracks amount of instructions done, cache misses etc
