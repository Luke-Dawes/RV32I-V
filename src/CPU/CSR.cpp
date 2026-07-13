#include "CSR.h"
#include <cstdint>


uint32_t CSR::read(uint16_t addr) const {
	return csr[addr];
}

void CSR::write(uint16_t addr, uint32_t value) {
	csr[addr] = value;
}

CSR::CSR() {
	csr[CSR_LOCATIONS::MSTATUS] = 0;
	csr[CSR_LOCATIONS::MTVEC] = 0;
	csr[CSR_LOCATIONS::MEPC] = 0;
	csr[CSR_LOCATIONS::MCAUSE] = 0;
	csr[CSR_LOCATIONS::MTVAL] = 0;
	csr[CSR_LOCATIONS::MIE] = 0;
	csr[CSR_LOCATIONS::MIP] = 0;
	csr[CSR_LOCATIONS::MISA] = (1u << 30) | (1u << ('I' - 'A'));
}