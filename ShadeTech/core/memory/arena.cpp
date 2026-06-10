#include "arena.h"

#include <stdlib.h>

namespace shd {

Arena::Arena(uint8* arenaBase, uint64 capacity) :
    base(arenaBase),
    capacity(capacity)
{
}

Result<Arena, bool> Arena::CreateFromLibC(uint64 capacity)
{
    void* base = malloc(capacity);
    if (base == nullptr) {
        return ErrorResult{ false };
    }

    return Arena{ (uint8*)base, capacity };
}

}
