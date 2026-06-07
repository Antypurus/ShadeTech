#pragma once

#include <core/types.h>

struct Arena
{
    uint8* base;
    uint64 offset;
    uint64 capacity;
};

namespace shd::memory {
/**
 * @brief Creates an arena based on a pre-allocated backing buffer
 *
 * @param arenaBase backing bufferr
 * @param capacity buffer/arena capacity
 * @return Corresponding Arena
 */
Arena CreateArena(uint8* arenaBase, uint64 capacity);

/**
 * @brief Creates an arena by asking libc malloc to allocate
 * a backing buffer with the corresponding capacity.
 *
 * @param capacity capacity to ask for
 * @return Arena allocated by libc-malloc or with 0 capacity
 * if the allocation failed.
 */
Arena CreateArenaFromLibC(uint64 capacity);
}
