#pragma once

#include <core/types.h>

namespace SHD {

class atomic_uint
{
private:
    volatile u64 m_value = 0;

public:
    atomic_uint() = default;
    atomic_uint(u64 initial_value);

    u64 increment();
    u64 decrement();
    u64 operator*() { return this->m_value; };
};

}
