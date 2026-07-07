#pragma once
#include <string>
#include <cstdint>
#include <vector>

class CPU;


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

struct Elf32_Shdr
{
    uint32_t sh_name;      // Offset into the section name string table
    uint32_t sh_type;      // Section type
    uint32_t sh_flags;     // Section flags
    uint32_t sh_addr;      // Virtual address in memory
    uint32_t sh_offset;    // Offset in the file
    uint32_t sh_size;      // Size of the section
    uint32_t sh_link;      // Link to another section
    uint32_t sh_info;      // Extra information
    uint32_t sh_addralign; // Required alignment
    uint32_t sh_entsize;   // Size of entries (if section is a table)
};

struct Elf32_Sym
{
    uint32_t st_name;
    uint32_t st_value;
    uint32_t st_size;
    uint8_t  st_info;
    uint8_t  st_other;
    uint16_t st_shndx;
};

struct Symbol {
    uint32_t address;
    uint32_t size;
    uint8_t type;
    std::string name;
};

constexpr uint32_t SHT_NULL = 0;
constexpr uint32_t SHT_PROGBITS = 1;
constexpr uint32_t SHT_SYMTAB = 2;
constexpr uint32_t SHT_STRTAB = 3;

constexpr auto STT_NOTYPE = 0;
constexpr auto STT_OBJECT = 1;
constexpr auto STT_FUNC = 2;
constexpr auto STT_SECTION = 3;


class Elf_Loader {
public:
	Elf_Loader() = default;

	void load(CPU& cpu, const std::string& path);

    std::vector<Symbol> symbols;
    std::vector<Elf32_Shdr> sections;

private:

    Elf32_Ehdr header;
    std::vector<Elf32_Phdr> program_headers;

    bool check_header() const;
    void load_symbols(std::ifstream& file);

};