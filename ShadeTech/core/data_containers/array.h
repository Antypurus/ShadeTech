#pragma once

#include <core/types.h>
#include <core/utils/memory/memory.h>

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
            this->resize(this->m_capacity * 2);
        }

        new (&this->m_array[this->m_size - 1]) T(value);
        this->m_size++;
    }

    T& operator[](usize index)
    {
        assert(index < this->m_size);
        return this->m_array[index];
    }
};

}
