#include "memory.h"

#include <core/types.h>
#include <core/utils/assert.h>
#include <cstdlib>

#include "stdlib.h"
#include "string.h"

namespace SHD {

void copy(void* source, usize size, void* destination, usize offset)
{
    ::memcpy((char*)destination + offset, source, size);
}

void memset(void* ptr, usize size, u8 value)
{
    ::memset(ptr, value, size);
}

/*
 * =========== System Allocator =============
 */

system_allocator system_allocator::s_instance;

system_allocator& system_allocator::instance()
{
    return s_instance;
}

void* system_allocator::alloc(usize size)
{
    return ::malloc(size);
}

void system_allocator::free(void* ptr, usize /*size*/)
{
    ::free(ptr);
}

/*
 * =========== Arena Allocator =============
 */

void* arena::alloc(usize size)
{
    ASSERT((this->m_used + size) <= this->m_capacity, "Allocation does not fit within remaining arena capacity");
    u8* ptr = this->m_arena + this->m_used;
    this->m_used += size;
    return ptr;
}

void arena::free(void* ptr, usize size)
{
    ASSERT(ptr != nullptr, "Attempting to deallocate null pointer");
    if ((this->m_arena + this->m_used - size) == ptr) {
        this->m_used -= size;
    }
}

}
