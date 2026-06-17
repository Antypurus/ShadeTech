#include "arena.h"

#include <stdlib.h>
#include <utility>

namespace shd {

Arena::Arena(uint8* arenaBase, uint64 capacity) :
    base(arenaBase),
    capacity(capacity)
{
}

Arena::Arena(uint8* arenaBuffer, uint64 arenaCapacity, uint64 arenaOffset) :
    base(arenaBuffer),
    capacity(arenaCapacity),
    offset(arenaOffset)
{
}

Arena::Arena(Arena&& other) :
    base(other.base),
    capacity(other.capacity),
    offset(other.offset)
{
    other.base = nullptr;
    other.offset = 0;
    other.capacity = 0;
}

Result<Arena, bool> Arena::CreateFromLibC(uint64 capacity)
{
    void* base = malloc(capacity);
    if (base == nullptr) {
        return ErrorResult{ false };
    }

    return std::move(Arena((uint8*)base, capacity));
}

Result<Arena, bool> Arena::CopyArena(Arena& backingArena)
{
    auto* newArenaBuffer = backingArena.Allocate<uint8>(this->capacity);
    if (newArenaBuffer == nullptr) {
        return ErrorResult(false);
    }

    return std::move(Arena(newArenaBuffer, this->capacity, this->offset));
}

Result<Arena, bool> Arena::CopyArenaUsingLibc()
{
    return ErrorResult(false);
}

}
