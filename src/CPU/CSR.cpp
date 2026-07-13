#include "CSR.h"
#include <cstdint>


uint32_t CSR::read(uint16_t addr) const {
	return csr[addr];
}

void CSR::write(uint16_t addr, uint32_t value) {
	csr[addr] = value;
}