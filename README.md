# RV32I

simple attempt at simulating a RISC-V (RV32I)

https://docs.riscv.org/reference/isa/unpriv/rv32.html - documentation

https://msyksphinz-self.github.io/riscv-isadoc/html/rvi.html - full link of all functions

https://dejazzer.com/coen2710/lectures/RISC-V-Reference-Data-Green-Card.pdf - cheat sheet

https://www.cs.sfu.ca/~ashriram/Courses/CS295/assets/notebooks/RISCV/RISCV_CARD.pdf - another cheat sheet



# long term aim

build this, with the base 40 instructions 

build a like interpreter which translates python or c++ or some sort of psuedocode to these instructions.

add pipelining with threading 

probably rewrite the emulator with uint64_t for each thing in ram

write a memory allocator properly? - add paging or segmentation - this is more OS than cpu but dont have another good project where i can try it 

as this is an emulator for a processor - mainly affecting alu and pc etc, could you make manual control signals/a control unit and then make a whole pc?

