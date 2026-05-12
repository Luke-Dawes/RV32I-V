#pragma once
#include <cstdint>
#include "../../CPU/Memory.h"

void sb(CPU& cpu, Decoded_instruction& ins);
void sh(CPU& cpu, Decoded_instruction& ins);
void sw(CPU& cpu, Decoded_instruction& ins);
