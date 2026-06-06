#pragma once

#include <core/types.h>

struct Arena {
    uint8* base;
    uint64 offset;
    uint64 capacity;
};

namespace shd::memory
{

    Arena CreateArena(uint8* arenaBase, uint64 capacity);


    Arena CreateArenaFromLibC(uint64 capacity);

}
