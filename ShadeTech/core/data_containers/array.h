#pragma once

#include <core/types.h>
#include <core/utils/assert.h>
#include <core/utils/memory/memory.h>
#include <core/utils/move.h>

#include <iostream>

namespace SHD {

template<typename T>
struct DynArray
{
private:
    allocator* m_allocator = nullptr;
    T* m_array = nullptr;
    usize m_size = 0;
    usize m_capacity = 0;

public:
    DynArray(allocator* allocator = &system_allocator::instance()) :
        m_allocator(allocator) {};

    DynArray(usize capacity, allocator* allocator = &system_allocator::instance()) :
        m_allocator(allocator)
    {
        this->m_array = allocator->allocate<T>(capacity);
        this->m_capacity = capacity;
    }

    ~DynArray()
    {
        if (this->m_array != nullptr) {
            this->m_allocator->free(this->m_array, this->m_capacity * sizeof(T));
            this->m_allocator = nullptr;
            this->m_capacity = 0;
            this->m_size = 0;
        }
    }

    DynArray(const DynArray& other) { *this = other; }

    DynArray(DynArray&& other) { *this = move(other); }

    DynArray& operator=(const DynArray& other)
    {
        if (&other == this)
            return *this;

        this->~DynArray();

        this->m_allocator = other.m_allocator;
        this->m_array = this->m_allocator->allocate<T>(other.m_capacity * sizeof(T));
        this->m_capacity = other.m_capacity;
        this->m_size = other.m_size;
        copy(other.m_array, other.m_size * sizeof(T), this->m_array);

        return *this;
    }

    DynArray& operator=(DynArray&& other)
    {
        if (&other == this)
            return *this;

        this->~DynArray();

        this->m_allocator = other.m_allocator;
        this->m_array = other.m_array;
        this->m_capacity = other.m_capacity;
        this->m_size = other.m_capacity;

        other.m_array = nullptr;
        other.m_capacity = 0;
        other.m_size = 0;

        return *this;
    }

    bool operator==(const DynArray& other)
    {
        if (this->m_size != other.m_size)
            return false;

        for (size_t i = 0; i < this->m_size; ++i) {
            if (this->m_array[i] != other.m_array[i])
                return false;
        }

        return true;
    }

    usize length() const { return this->m_size; }

    T* begin() { return this->m_array; }

    T* end() { return (this->m_array + this->m_size); }

    void resize(usize new_size)
    {
        T* new_buffer = this->m_allocator->allocate<T>(new_size * sizeof(T));
        if (this->m_array != nullptr) {
            copy(this->m_array, this->m_size * sizeof(T), new_buffer);
            this->m_allocator->free(this->m_array, this->m_capacity * sizeof(T));
        }

        this->m_array = new_buffer;
        this->m_capacity = new_size;
    }

    void push_back(const T& value)
    {
        if (this->m_size + 1 > this->m_capacity) {
            usize new_capacity = this->m_capacity == 0 ? 1 : this->m_capacity * 2;
            this->resize(new_capacity);
        }

        new (&this->m_array[this->m_size]) T(value);
        this->m_size++;
    }

    void clear()
    {
        memset(this->m_array, this->m_capacity * sizeof(T), 0);
        this->m_size = 0;
    }

    T& operator[](usize index)
    {
        ASSERT(index < this->m_size, "Out of bounds array access");
        return this->m_array[index];
    }
};

}
