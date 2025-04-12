#pragma once

#include "types.h"

#include <mutex>
#include <unordered_map>

namespace SHD {

void* allocate_memory(usize size);
void free_memory(void* ptr);
void copy(void* source, usize size, void* destination, usize offset = 0);
void memset(void* ptr, usize size, u8 value);

template<typename T, typename AlignT>
T to_alignment(T value, AlignT alignment)
{
    return (alignment - (value % alignment)) % alignment;
}

template<typename T, typename AlignT>
T align(T value, AlignT alignment)
{
    return value + to_alignment(value, alignment);
}

struct allocator;
struct allocation_registry
{
private:
    static allocation_registry s_instance;
    std::unordered_map<u8*, allocator*> m_registry;
    std::mutex m_registry_guard;

public:
    static allocation_registry& instance();

    allocation_registry() = default;
    void register_allocation(u8* ptr, allocator* allocator);
    void remove_allocation(u8* ptr);
};

struct allocator
{
protected:
    u8* m_allocator_base = nullptr;
    allocator* m_parent = nullptr;
    const char* m_name = "";

public:
    allocator(u8* base, allocator* parent = nullptr, const char* name = "");
    virtual ~allocator();
};

struct system_allocator : public allocator
{
private:
    static system_allocator s_instance;

public:
    static system_allocator& instance();

    system_allocator();
};

struct arena
{
public:
    u8* m_arena = nullptr;
    usize m_capacity = 0;
    usize m_used = 0;
    u16 m_alignment = 0;

private:
    arena() = default;
    arena(u8* ptr, usize capacity, u16 alignment = 0);

    u8* allocate_buffer(usize size);
};

}
