#pragma once

#include <core/result.h>
#include <core/types.h>

namespace shd {

struct Arena
{
public:
    uint8* base = nullptr;
    uint64 offset = 0;
    uint64 capacity = 0;

public:
    /**
     * @brief Creates an arena by asking libc malloc to allocate
     * a backing buffer with the corresponding capacity.
     *
     * @param capacity capacity to ask for
     * @return Arena allocated by libc-malloc or with 0 capacity
     * if the allocation failed.
     */
    static Result<Arena, bool> CreateFromLibC(uint64 capacity);

public:
    Arena(uint8* arenaBuffer, uint64 arenaCapacity);
    ~Arena() = default;
};

}
