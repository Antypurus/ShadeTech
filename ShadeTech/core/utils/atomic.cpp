#include "atomic.h"

#include <core/platform.h>

#if COMPILER_MSVC
#include <intrin.h>
#endif

namespace SHD {

atomic_uint::atomic_uint(u64 initial_value) :
    m_value(initial_value)
{
}

void atomic_uint::increment()
{
#if COMPILER_MSVC
    _InterlockedIncrement64((volatile i64*)&this->m_value);
#else
    __sync_fetch_and_add(&this->m_value, 1);
#endif
}

void atomic_uint::decrement()
{
#if COMPILER_MSVC
    _InterlockedDecrement64((volatile i64*)&this->m_value);
#else
    __sync_fetch_and_sub(&this->m_value, 1);
#endif
}

}
