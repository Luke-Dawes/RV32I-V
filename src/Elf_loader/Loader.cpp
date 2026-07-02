#include "Loader.h"
#include <istream>
#include <fstream>
#include "../Memory/Memory.h"
#include "../CPU/CPU.h"

bool Elf_Loader::checkHeader() const {
	return true;
}

void Elf_Loader::load(CPU& cpu, const std::string& path) {

	std::ifstream file (path, std::ios::binary); //open file as a binary file

	file.read(reinterpret_cast<char*>(&header), sizeof(header)); //read the first 52 (sizeof(header)) into &header as char* (bytes)

	if (!checkHeader()) {
		return;
	}

	program_headers.resize(header.e_phnum); //resize for the number of program headers (e._phnum)

	file.seekg(header.e_phoff); //move the file pointer to e_phoff

	file.read(reinterpret_cast<char*>(program_headers.data()), header.e_phnum * sizeof(Elf32_Phdr)); //read all the headers into the vector.

	for (auto& ph : program_headers) {

		if (ph.p_type != 1) { //1 is PT_LOAD which needs to be loaded 
			continue;
		}

		file.seekg(ph.p_offset); //move to where the data is in the file

		uint32_t ram_offset = ph.p_vaddr - cpu.memory.RAM_BASE; //find where it should go in ram

		file.read(reinterpret_cast<char*>(&cpu.memory.data()[ram_offset]), ph.p_filesz); //read the data straight into ram[offset]

		if (ph.p_memsz > ph.p_filesz) { //if there needs to be memory which is initalised to 0
			uint32_t bss_offset = ram_offset + ph.p_filesz; //where the .bss section starts in ram
			uint32_t bss_size = ph.p_memsz - ph.p_filesz; //how many 0 to fill the .bss

			std::fill_n(&cpu.memory.data()[bss_offset], bss_size, 0); //fill it 
			
		}
	}
}