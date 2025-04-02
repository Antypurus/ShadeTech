#include "core/memory.h"

#include "stdlib.h"
#include "string.h"

namespace SHD {
void* AllocateMemory(usize size)
{
    return malloc(size);
}

void free_memory(void* ptr)
{
    free(ptr);
}

void Copy(void* source, usize size, void* destination, usize offset)
{
    memcpy((char*)destination + offset, source, size);
}

}
