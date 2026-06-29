#pragma once
#include <string>
#include <cstdint>
#include <vector>


struct Elf32_Ehdr {
    unsigned char e_ident[16]; //"magic number" (shows its an elf file)
    //also contains 32 v 64 bit, endianness and elf version

    uint16_t e_type;           //what type of file - executable vs .o/.so
    uint16_t e_machine;        //what type of assembly (arm x86 vs risc-v)
    uint32_t e_version;        //elf format version
    uint32_t e_entry;          //entry point of executable
    uint32_t e_phoff;          //offset to the program header table
    uint32_t e_shoff;          //offset to the section header table
    uint32_t e_flags;          //architecture specific flags - ignore
    uint16_t e_ehsize;         //size of the elf header
    uint16_t e_phentsize;      //size of one program header
    uint16_t e_phnum;          //number of program headers
    uint16_t e_shentsize;      //size of one section header
    uint16_t e_shnum;          //number of section headers
    uint16_t e_shstrndx;       //index of section containing section names
};

struct Elf32_Phdr {
    uint32_t p_type;           //what kind of segment - we only want load wich is 1
    uint32_t p_offset;         //offset to where the data begins
    uint32_t p_vaddr;          //address where it should be placed
    uint32_t p_paddr;          //physical address  (same address as above)
    uint32_t p_filesz;         //how many bytes the segment contains
    uint32_t p_memsz;          //how much memory the segment should occupy after loading
    uint32_t p_flags;          //permissions? 
    uint32_t p_align;          //alignment of the segment
};

class Elf_Loader {
public:
	Elf_Loader() = default;

	void load(const std::string& path);

private:

    Elf32_Ehdr header;
    std::vector<Elf32_Phdr> program_headers;

    bool checkHeader() const;

};