#include "arena.h"
#include "core/types.h"

#include <stdlib.h>

namespace shd::memory {

Arena CreateArena(uint8* arenaBase, uint64 capacity)
{
    return Arena{
        .base = arenaBase,
        .offset = 0,
        .capacity = capacity,
    };
}

Arena CreateArenaFromLibC(uint64 capacity)
{
    void* base = malloc(capacity);
    if (base == nullptr) {
        return Arena{
            .base = nullptr,
            .offset = 0,
            .capacity = 0,
        };
    }
    return Arena{
        .base = (uint8*)base,
        .offset = 0,
        .capacity = capacity,
    };
}

}
