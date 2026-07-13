#include <cstdint>


enum Privilage_mode {
	USER = 0,
	SUPERVISOR = 1,
	MACHINE = 2,
};

class CSR {
public:

	uint32_t read(uint16_t csr) const;
	void write(uint16_t csr, uint32_t value);

private:
	uint32_t csr[4096];
};