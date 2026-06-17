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
    Arena(uint8* arenaBuffer, uint64 arenaCapacity, uint64 offset);
    ~Arena() = default;
    Arena(Arena&& other);

    Result<Arena, bool> CopyArena(Arena& backingArena);
    Result<Arena, bool> CopyArenaUsingLibc();

    template<typename T>
    T* Allocate(uint64 bytes)
    {
        return this->base;
    }

public:
    /* for now lets keep assignment operator disabled
     * since we cant auto free its a bit unclear to me weather
     * this should be allowed, especially because it can lead
     * to some weird bugs if we dont properly account for this */
    Arena& operator=(Arena&& other) = delete;
    Arena& operator=(const Arena& other) = delete;

    /* we cant have a copy constructor due to lacking a
     * backing buffer/arena to allocate from */
    Arena(const Arena& other) = delete;
};

}
