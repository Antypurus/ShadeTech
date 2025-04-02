#pragma once

#include "Types.h"

namespace SHD {

void* AllocateMemory(usize size);
void free_memory(void* ptr);
void Copy(void* source, usize size, void* destination, usize offset = 0);

}
