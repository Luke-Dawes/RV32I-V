#include "Loader.h"
#include <istream>
#include <fstream>
#include "../Memory/Memory.h"
#include "../CPU/CPU.h"

bool Elf_Loader::check_header() const {

    // ELF magic
    if (header.e_ident[0] != 0x7F ||
        header.e_ident[1] != 'E' ||
        header.e_ident[2] != 'L' ||
        header.e_ident[3] != 'F')
        return false;

    // 32-bit ELF
    if (header.e_ident[4] != 1)      // ELFCLASS32
        return false;

    // Little endian
    if (header.e_ident[5] != 1)      // ELFDATA2LSB
        return false;

    // ELF version
    if (header.e_ident[6] != 1)
        return false;

    // Executable
    if (header.e_type != 2)          // ET_EXEC
        return false;

    // RISC-V
    if (header.e_machine != 243)     // EM_RISCV
        return false;

    // Current ELF version
    if (header.e_version != 1)
        return false;

    // Correct header size
    if (header.e_ehsize != sizeof(Elf32_Ehdr))
        return false;

    return true;
}

void Elf_Loader::load(CPU& cpu, const std::string& path) {

	std::ifstream file (path, std::ios::binary); //open file as a binary file

    if (!file.read(reinterpret_cast<char*>(&header), sizeof(header))) //read the first 52 (sizeof(header)) into &header as char* (bytes)
        return; 

	if (!check_header()) {
		return;
	}

	program_headers.resize(header.e_phnum); //resize for the number of program headers (e._phnum)

	file.seekg(header.e_phoff); //move the file pointer to e_phoff

    if (!file.read(reinterpret_cast<char*>(program_headers.data()), header.e_phnum * sizeof(Elf32_Phdr))) //read all the headers into the vector.
        return;

	for (auto& ph : program_headers) {

		if (ph.p_type != 1) { //1 is PT_LOAD which needs to be loaded 
			continue;
		}

		file.seekg(ph.p_offset); //move to where the data is in the file

		uint32_t ram_offset = ph.p_vaddr - cpu.memory.RAM_BASE; //find where it should go in ram

        if (!file.read(reinterpret_cast<char*>(&cpu.memory.data()[ram_offset]), ph.p_filesz)) //read the data straight into ram[offset]
            return;
        
		if (ph.p_memsz > ph.p_filesz) { //if there needs to be memory which is initalised to 0
			uint32_t bss_offset = ram_offset + ph.p_filesz; //where the .bss section starts in ram
			uint32_t bss_size = ph.p_memsz - ph.p_filesz; //how many 0 to fill the .bss

			std::fill_n(&cpu.memory.data()[bss_offset], bss_size, 0); //fill it 
			
		}
	}

    cpu.PC = header.e_entry;
    cpu.registers[2] = cpu.memory.RAM_BASE + cpu.memory.memory_size - 16;

    //debugging - load symbols
    load_symbols(file);
    file.close();

}

void Elf_Loader::load_symbols(std::ifstream& file) {

    if (header.e_shoff == 0 || header.e_shnum == 0) //if no symbol table
        return;

    sections.resize(header.e_shnum); //make sections the size of however many sections there are

    file.seekg(header.e_shoff); //move it in the file to the symbol table

    if (!file.read(reinterpret_cast<char*>(sections.data()), sections.size() * sizeof(Elf32_Shdr))) //read it into sections
        return;

    Elf32_Shdr* symtab = nullptr;
    Elf32_Shdr* strtab = nullptr;

    for (auto& sh : sections) //finds the symbol table
    {
        if (sh.sh_type == SHT_SYMTAB)
        {
            symtab = &sh; //set symtab to the section with the symbol table
            strtab = &sections[sh.sh_link]; //link to index of the string table containing the names
            break;
        }
    }

    if (!symtab || !strtab) //if either of these dont exist its illegal
        return;

    //symbols as strings
    std::vector<char> strings(strtab->sh_size); 

    file.seekg(strtab->sh_offset); //find the string table 

    if (!file.read(strings.data(), strings.size())) //read the string table into strings
        return;

    size_t count = symtab->sh_size / sizeof(Elf32_Sym); //calc how many symbols

    std::vector<Elf32_Sym> elf_symbols(count); //create a vertor of my symbols

    file.seekg(symtab->sh_offset); //find the symbol table

    if (!file.read(reinterpret_cast<char*>(elf_symbols.data()), symtab->sh_size)) //read the symbol table
        return;

    symbols.clear(); //should be empty but just to make sure no past things get in the way

    for (const auto& s : elf_symbols)
    { 
        if (s.st_name == 0) //skip unnamed symbols
            continue;

        Symbol sym; //create a symbol

        sym.address = s.st_value;
        sym.size = s.st_size;
        sym.type = s.st_info & 0x0F;
        sym.name = &strings[s.st_name];

        symbols.push_back(std::move(sym));
    }
}
