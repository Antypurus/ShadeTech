#include "core/memory.h"

#include "stdlib.h"
#include "string.h"

namespace SHD {
void* allocate_memory(usize size)
{
    return malloc(size);
}

void free_memory(void* ptr)
{
    free(ptr);
}

void copy(void* source, usize size, void* destination, usize offset)
{
    memcpy((char*)destination + offset, source, size);
}

void memset(void* ptr, usize size, u8 value)
{
    ::memset(ptr, value, size);
}

}
