#pragma once

#include <core/types.h>
#include <core/utils/memory/memory.h>
#include <core/utils/move.h>
#include <core/utils/optional.h>

namespace SHD {

template<typename T>
struct ring_buffer
{
private:
    allocator* m_allocator = nullptr;
    T* m_buffer = nullptr;
    usize m_ring_element_count = 0;
    usize m_head = 0; // where we push into
    usize m_tail = 0; // where we pop from

public:
    ring_buffer(usize element_count, allocator* allocator = &system_allocator::instance()) :
        m_allocator(allocator),
        m_ring_element_count(element_count)
    {
        this->m_buffer = this->m_allocator->allocate<T>(element_count * sizeof(T));
    }

    ~ring_buffer()
    {
        if(this->m_allocator != nullptr && this->m_buffer != nullptr)
        {
            this->m_allocator->free(this->m_buffer, this->m_ring_element_count * sizeof(T));
            this->m_ring_element_count = 0;
            this->m_head = 0;
            this->m_tail = 0;
        }
    }

    ring_buffer(const ring_buffer& other):
        m_allocator(other.m_allocator),
        m_ring_element_count(other.m_ring_element_count),
        m_head(other.m_head),
        m_tail(other.m_head)
    {
        this->m_buffer = this->m_allocator->allocate<T>(this->m_ring_element_count * sizeof(T));
        copy(other.m_buffer, other.m_ring_element_count * sizeof(T), this->m_buffer);
    }

    ring_buffer& operator=(const ring_buffer& other)
    {
        if(this == &other)
            return *this;

        this->~ring_buffer();
        this->m_allocator = other.m_allocator;
        this->m_ring_element_count = other.m_ring_element_count;
        this->m_head = other.m_head;
        this->m_tail = other.m_tail;
        this->m_buffer = this->m_allocator->allocate<T>(this->m_ring_element_count * sizeof(T));
        copy(other.m_buffer, other.m_ring_element_count * sizeof(T), this->m_buffer);

        return *this;
    }

    ring_buffer(ring_buffer&& other):
        m_allocator(other.m_allocator),
        m_buffer(other.m_buffer),
        m_ring_element_count(other.m_ring_element_count),
        m_head(other.m_head),
        m_tail(other.m_tail)
    {
        other.m_allocator = nullptr;
        other.m_buffer = nullptr;
        other.m_ring_element_count = 0;
        other.m_head = 0;
        other.m_tail = 0;
    }

    ring_buffer& operator=(ring_buffer&& other)
    {
        if(this == &other)
            return *this;

        this->m_allocator = other.m_allocator;
        this->m_buffer = other.m_buffer;
        this->m_ring_element_count = other.m_ring_element_count;
        this->m_head = other.m_head;
        this->m_tail = other.m_tail;
        other.m_allocator = nullptr;
        other.m_buffer = nullptr;
        other.m_ring_element_count = 0;
        other.m_head = 0;
        other.m_tail = 0;

        return *this;
    }

    void push_element(const T& element)
    {
        new (&this->m_buffer[this->m_head]) T(element);
        this->increment_head();
    }

    void push_element(T&& element)
    {
        new (&this->m_buffer[this->m_head]) T(move(element));
        this->increment_head();
    }

    optional<T> pop()
    {
        if (this->m_tail == this->m_head) {
            return {};
        }

        T element = move(this->m_buffer[this->m_tail]);
        this->increment_tail();
        return { move(element) };
    }

private:
    void increment_head()
    {
        this->m_head = (this->m_head + 1) % this->m_ring_element_count;
        if (this->m_head == this->m_tail) {
            this->increment_tail();
        }
    }

    void increment_tail() { this->m_tail = (this->m_tail + 1) % this->m_ring_element_count; }
};

}
