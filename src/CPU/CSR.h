#pragma once
#include <cstdint>


namespace CSR_LOCATIONS
{
	constexpr uint16_t MSTATUS = 0x300;
	constexpr uint16_t MISA = 0x301;
	constexpr uint16_t MIE = 0x304;
	constexpr uint16_t MTVEC = 0x305;
	constexpr uint16_t MSCRATCH = 0x340;
	constexpr uint16_t MEPC = 0x341;
	constexpr uint16_t MCAUSE = 0x342;
	constexpr uint16_t MTVAL = 0x343;
	constexpr uint16_t MIP = 0x344;
}

enum Privilage_mode {
	USER = 0,
	SUPERVISOR = 1,
	MACHINE = 2,
};

class CSR {
public:

	CSR();

	uint32_t read(uint16_t addr) const;
	void write(uint16_t addr, uint32_t value);

	//uint32_t* mstatus = &csr[0x300];
	//uint32_t* misa = &csr[0x301];
	//uint32_t* mie = &csr[0x304];
	//uint32_t* mtvec = &csr[0x305];
	//uint32_t* mscratch = &csr[0x340];
	//uint32_t* mepc = &csr[0x341];
	//uint32_t* mcause = &csr[0x342];
	//uint32_t* mtval = &csr[0x343];
	//uint32_t* mip = &csr[0x344];


private:
	uint32_t csr[4096];

	

};