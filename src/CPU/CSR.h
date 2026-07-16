#pragma once
#include <cstdint>
#include <optional>


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

private:
	uint32_t csr[4096];
};