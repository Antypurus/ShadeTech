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

export void Copy(void* source, usize size, void* destination)
{
    memcpy(destination, source, size);
}

}
