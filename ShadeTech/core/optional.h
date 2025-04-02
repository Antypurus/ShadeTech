#pragma once

#include "assert.h"
#include "core/move.h"
#include "types.h"

namespace SHD {

template<typename T>
class optional
{
private:
    alignas(T) union
    {
        T m_value;
        u8 m_storage[sizeof(T)];
    };
    bool m_has_value = false;

public:
    optional() = default;

    optional(const T& value) :
        m_value(value),
        m_has_value(true)
    {
    }

    optional(T&& value) :
        m_value(move(value)),
        m_has_value(true)
    {
    }

    ~optional()
    {
        if (this->m_has_value) {
            this->m_value.~T();
        }
        this->m_has_value = false;
    }

    optional(const optional& other)
    {
        this->~optional();
        this->m_has_value = other.m_has_value;
        if (this->m_has_value) {
            new (&this->m_value) T(other.m_value);
        }
    }

    optional(optional&& other)
    {
        this->~optional();
        this->m_has_value = other.m_has_value;
        if (this->m_has_value) {
            new (&this->m_value) T(move(other.m_value));
        }
        other.m_has_value = false;
    }

    optional& operator=(const optional& other)
    {
        if (this == &other)
            return *this;

        this->~optional();
        this->m_has_value = other.m_has_value;
        if (this->m_has_value) {
            new (&this->m_value) T(other.m_value);
        }

        return *this;
    }

    optional& operator=(optional&& other)
    {
        if (this == &other)
            return *this;

        this->~optional();
        this->m_has_value = other.m_has_value;
        if (this->m_has_value) {
            new (&this->m_value) T(move(other.m_value));
        }
        other.m_has_value = false;

        return *this;
    }

    bool hasValue() const { return this->m_has_value; }

    T& getValue()
    {
        ASSERT(this->m_has_value, "Optional does not hold a value");
        return this->m_value;
    }

    T& operator*()
    {
        ASSERT(this->m_has_value, "Optional does not hold a value");
        return this->m_value;
    }
};

}
