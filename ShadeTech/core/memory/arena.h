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
    /**
     * @brief Constructor that receives a preallocated buffer
     * to cover the arena
     *
     * @param arenaBuffer arena backign buffer
     * @param arenaCapacity arena memory capacity in bytes
     */
    Arena(uint8* arenaBuffer, uint64 arenaCapacity);

    /**
     * @brief Constructor that receives a preallocated buffer
     * to cover the arena
     *
     * @param arenaBuffer arena backign buffer
     * @param arenaCapacity arena memory capacity in bytes
     * @param offset offset from the start of the supplied buffer to start the arena at
     *  - ArenaBasePtr = arenaBuffer + offset
     *  - ArenaCapacity = arenaCapacity
     */
    Arena(uint8* arenaBuffer, uint64 arenaCapacity, uint64 offset);

    ~Arena() = default;
    Arena(Arena&& other);

    /**
     * @brief Creates a copy of the calling arena(including its contents)
     * using the supplied arena to allocate the required memory
     *
     * @param backingArena Arena to allocate from to cover memory requirements
     */
    Result<Arena, bool> CopyArena(Arena& backingArena);

    /**
     * @brief Creates a copy of the calling arena (including its contents)
     * using LibC Malloc to allocate the memory required for the arena
     * copy.
     */
    Result<Arena, bool> CopyArenaUsingLibc();

    template<typename T>
    T* Allocate(uint64 bytes)
    {
        T* result = this->base + offset;
        this->offset += bytes;

        return result;
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
