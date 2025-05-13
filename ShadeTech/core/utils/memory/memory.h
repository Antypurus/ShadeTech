#pragma once

#include <core/types.h>

namespace SHD {

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

/*
 * =========== Allocator Interface =============
 */

#define DEFAULT_ALLOCATOR &system_allocator::instance()

struct allocator
{
public:
    virtual ~allocator() = default;

    template<typename T>
    T* allocate(usize size)
    {
        return (T*)this->alloc(size);
    }

    template<typename T>
    void free(T* ptr, usize size)
    {
        this->free((void*)ptr, size);
    }

protected:
    virtual void* alloc(usize size) = 0;
    virtual void free(void* ptr, usize size) = 0;
};

/*
 * =========== System Allocator =============
 */

struct system_allocator : public allocator
{
public:
    static system_allocator s_instance;

public:
    ~system_allocator() override = default;
    static system_allocator& instance();

private:
    system_allocator() = default;

protected:
    void* alloc(usize size) override;
    void free(void* ptr, usize size) override;
};

/*
 * =========== Arena Allocator =============
 */

struct arena : public allocator
{
private:
    u8* m_arena = nullptr;
    usize m_used = 0;
    usize m_capacity = 0;

public:
    arena() = default;
    ~arena() override = default;

protected:
    void* alloc(usize size) override;
    void free(void* ptr, usize size) override;
};

}
