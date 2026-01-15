#pragma once

#include <core/types.h>
#include <core/utils/memory/memory.h>
#include <core/utils/assert.h>

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

    T* begin() { return this->m_array; }

    T* end() { return (this->m_array + this->m_size); }

    void resize(usize new_size)
    {
        T* new_buffer = this->m_allocator->allocate<T>(new_size);
        copy(this->m_array, this->m_size, new_buffer, 0);
        this->m_allocator->free(this->m_array, this->m_capacity);

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

    T& operator[](usize index)
    {
        ASSERT(index < this->m_size, "Out of bounds array access");
        return this->m_array[index];
    }
};

}
