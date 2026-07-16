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

==================================================
Project 1 - RISC-V Emulator (C++)
==================================================

### Milestone 1 - CPU
✅ RV32I base ISA
✅ M extension (multiply/divide)
✅ Instruction testing

### Milestone 2 - Program Loading
✅ ELF loader
✅ Load program segments
✅ Initialise PC
✅ Initialise SP
✅ Symbol loading

### Milestone 3 - Debugger
✅ Single step
✅ Continue
✅ Breakpoints
✅ Register viewer
✅ Memory viewer
⏳ Symbol lookup

### Milestone 4 - Privileged Architecture (M-mode)
✅ CSR registers
✅ CSR instructions
⏳ mstatus handling
⏳ mtvec
⏳ mepc
⏳ mcause
⏳ mtval
⏳ mie/mip

### Milestone 5 - Traps
✅ Trap entry
✅ Save MEPC
✅ Save MCAUSE
✅ Save MTVAL
✅ Update MSTATUS
✅ Jump to MTVEC

### Exceptions:
✅ Illegal instruction
✅ ECALL
✅ EBREAK
✅ Load/store faults
✅ Misaligned access

⏳ MRET

### Milestone 6 - Hardware
 Memory mapped devices
 UART console output
 Timer device
 Timer interrupts

### Milestone 7 - Kernel Ready Emulator
✅ Load kernel ELF
✅ Execute boot code
 Kernel prints to UART


==================================================
Project 2 - Mini Kernel (C + Assembly)
==================================================

### Milestone 8 - Boot
✅ boot.S
✅ Stack setup
kernel_main()

### Milestone 9 - Console
 UART driver
 putchar()
 printf()
 Logging

### Milestone 10 - Kernel Traps
 trap.S
 Save registers
 Restore registers
 Trap handler in C
 Exception handling
 Interrupt handling
 MRET return

### Milestone 11 - Memory Management
 Memory map
 Physical page allocator
 Free pages
 kmalloc()
 kfree()

### Milestone 12 - Virtual Memory
 Sv39 page tables
 Address translation
 Map kernel memory
 Enable paging
 Page faults

### Milestone 13 - Processes
 PCB
 Create process
 Destroy process
 Process states

### Milestone 14 - Scheduler
 Timer scheduling
 Round-robin scheduler
 Context switching

### Milestone 15 - System Calls
 syscall handler
 write()
 read()
 exit()
 yield()
 sleep()
 Memory allocation


==================================================
Project 3 - User Mode
==================================================

### Milestone 16 - Supervisor Mode
 S-mode privilege
 S-mode CSRs
 SRET
 Trap delegation

### Milestone 17 - User Programs
 User ELF loading
 User page tables
 User/kernel separation
 User stack
 Basic libc


==================================================
Optional
==================================================

 Disk device
 Filesystem
 GPU/framebuffer
 Networking
 Cache simulator
 Performance statistics
