#pragma once

#include "Types.h"
#include "core/move.h"
#include "assert.h"

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
    bool m_hasValue = false;

public:
    optional() = default;

    optional(const T& value) :
        m_value(value),
        m_hasValue(true)
    {
    }

    optional(T&& value) :
        m_value(move(value)),
        m_hasValue(true)
    {
    }

    ~optional()
    {
        if (this->m_hasValue) {
            this->m_value.~T();
        }
        this->m_hasValue = false;
    }

    optional(const optional& other)
    {
        this->~optional();
        this->m_hasValue = other.m_hasValue;
        if (this->m_hasValue) {
            new (&this->m_value) T(other.m_value);
        }
    }

    optional(optional&& other)
    {
        this->~optional();
        this->m_hasValue = other.m_hasValue;
        if (this->m_hasValue) {
            new (&this->m_value) T(move(other.m_value));
        }
        other.m_hasValue = false;
    }

    optional& operator=(const optional& other)
    {
        if (this == &other)
            return *this;

        this->~optional();
        this->m_hasValue = other.m_hasValue;
        if (this->m_hasValue) {
            new (&this->m_value) T(other.m_value);
        }

        return *this;
    }

    optional& operator=(optional&& other)
    {
        if (this == &other)
            return *this;

        this->~optional();
        this->m_hasValue = other.m_hasValue;
        if (this->m_hasValue) {
            new (&this->m_value) T(move(other.m_value));
        }
        other.m_hasValue = false;

        return *this;
    }

    bool hasValue() const { return this->m_hasValue; }

    T& getValue()
    {
        ASSERT(this->m_hasValue, "Optional does not hold a value");
        return this->m_value;
    }

    T& operator*()
    {
        ASSERT(this->m_hasValue, "Optional does not hold a value");
        return this->m_value;
    }
};

}
