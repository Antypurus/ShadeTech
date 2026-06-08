#pragma once

#include <core/result.h>
#include <core/types.h>

namespace shd {

struct Arena
{
    uint8* base;
    uint64 offset;
    uint64 capacity;

    /**
     * @brief Creates an arena based on a pre-allocated backing buffer
     *
     * @param arenaBase backing bufferr
     * @param capacity buffer/arena capacity
     * @return Corresponding Arena
     */
    static Arena Create(uint8* arenaBase, uint64 capacity);

    /**
     * @brief Creates an arena by asking libc malloc to allocate
     * a backing buffer with the corresponding capacity.
     *
     * @param capacity capacity to ask for
     * @return Arena allocated by libc-malloc or with 0 capacity
     * if the allocation failed.
     */
    static Result<Arena, bool> CreateFromLibC(uint64 capacity);
};

}
