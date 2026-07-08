# RV32I emulator and C kernal to run on it

### useful links

https://docs.riscv.org/reference/isa/unpriv/rv32.html - documentation

https://msyksphinz-self.github.io/riscv-isadoc/html/rvi.html - full link of all functions

https://dejazzer.com/coen2710/lectures/RISC-V-Reference-Data-Green-Card.pdf - cheat sheet

https://www.cs.sfu.ca/~ashriram/Courses/CS295/assets/notebooks/RISCV/RISCV_CARD.pdf - another cheat sheet

https://docs.riscv.org/reference/isa/v20260120/unpriv/m-st-ext.html m extention

https://gist.github.com/x0nu11byt3/bcb35c3de461e5fb66173071a2379779 elf loader



# plan - with C and C++
make the kernal compile into an elf file, and then have my emulator run that elf file.

## Project 1 - RISC-V Emulator (C++)
### CPU
✅ Implement RV32I base ISA

✅ Build assembler

✅ Encode assembly into machine code

⏳ Finish testing M extension (multiply/divide)

### Program Loading
✅ Implement ELF loader

✅ Load program segments into RAM

✅ Initialise program counter (PC)

✅ Initialise stack pointer (SP)

✅ Support symbols for debugging

### Debugger
✅ Single step

✅ Continue

✅ Breakpoints

✅ Register viewer

✅ Memory viewer

### Disassembler
⏳ Symbol lookup

### privileged architecture - machine mode
Implement CSR registers

CSR instructions 

Implement Trap handling

Implement exceptions

ECALL / EBREAK 

MRET

Implement interrupts

### supervisor mode 
s-mode CSR instructions

SRET

user mode? 

### Memory System
Physical RAM

Memory-mapped devices

### Sv39 virtual memory
Address translation

Page table walk

Sv39

Page faults

TLB (optional)

Hardware Devices
UART (console output)

Timer interrupt

(Optional) Disk device

(Optional) Simple GPU/framebuffer

### Performance
Cache simulator

Instruction counter

Cycle counter

Cache statistics

Branch statistics

Performance report


## Project 2 - Mini Kernel (C)
### Boot
Boot assembly (boot.S)

Initialise stack

Jump to kernel_main()

### Console
UART driver

printf()

Simple logging

Trap Handling

Trap handler

Exception handling

Interrupt handling

Return from trap

### Memory Management
Physical page allocator

Kernel heap (kmalloc)

Free memory

Memory map

Virtual Memory

Create page tables

Map kernel memory

Enable paging

User address spaces

### Processes
Process Control Block (PCB)

Create process

Destroy process

Process states

### Scheduler
Timer-based scheduling

Round-robin scheduler

Context switching

### System Calls

write
read

exit

yield

sleep

Memory allocation
