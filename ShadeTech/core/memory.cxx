module;

#include "Types.h"
#include "stdlib.h"
#include "string.h"

export module core.memory;

namespace SHD {

export void* AllocateMemory(usize size)
{
    return malloc(size);
}

export void free_memory(void* ptr)
{
    free(ptr);
}

export void Copy(void* source, usize size, void* destination, usize offset = 0)
{
    memcpy((char*)destination + offset, source, size);
}

}
