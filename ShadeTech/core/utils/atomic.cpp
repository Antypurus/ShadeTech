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

u64 atomic_uint::increment()
{
#if COMPILER_MSVC
    return _InterlockedIncrement64((volatile i64*)&this->m_value);
#else
    return __sync_fetch_and_add(&this->m_value, 1) + 1;
#endif
}

u64 atomic_uint::decrement()
{
#if COMPILER_MSVC
    return _InterlockedDecrement64((volatile i64*)&this->m_value);
#else
    return __sync_fetch_and_sub(&this->m_value, 1) - 1;
#endif
}

}
