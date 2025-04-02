#pragma once

#include "types.h"

namespace SHD {

void* allocate_memory(usize size);
void free_memory(void* ptr);
void copy(void* source, usize size, void* destination, usize offset = 0);
void memset(void* ptr, usize size, u8 value);

}
