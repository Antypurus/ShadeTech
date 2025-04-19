#include "memory.h"

#include <core/types.h>
#include <core/utils/assert.h>

#include "stdlib.h"
#include "string.h"

namespace SHD {

allocation_registry allocation_registry::s_instance;
system_allocator system_allocator::s_instance;

void* allocate_memory(usize size)
{
    return malloc(size);
}

void free_memory(void* ptr)
{
    free(ptr);
}

void copy(void* source, usize size, void* destination, usize offset)
{
    memcpy((char*)destination + offset, source, size);
}

void memset(void* ptr, usize size, u8 value)
{
    ::memset(ptr, value, size);
}

allocation_registry& allocation_registry::instance()
{
    return allocation_registry::s_instance;
}

void allocation_registry::register_allocation(u8* ptr, allocator* allocator)
{
    this->m_registry_guard.lock();
    this->m_registry[ptr] = allocator;
    this->m_registry_guard.unlock();
}

void allocation_registry::remove_allocation(u8* ptr)
{
    this->m_registry_guard.lock();
    this->m_registry.erase(this->m_registry.find(ptr));
    this->m_registry_guard.unlock();
}

allocator::allocator(u8* base, allocator* parent, const char* name) :
    m_allocator_base(base),
    m_parent(parent),
    m_name(name)
{
    allocation_registry::instance().register_allocation(this->m_allocator_base, this);
}

allocator::~allocator()
{
    if (this->m_parent == nullptr) {
        // remove from allocation list
        allocation_registry::instance().remove_allocation(this->m_allocator_base);
    } else {
        // give base back to parent
        allocation_registry::instance().register_allocation(this->m_allocator_base, this->m_parent);
    }
}

system_allocator& system_allocator::instance()
{
    return system_allocator::s_instance;
}

system_allocator::system_allocator() :
    allocator(nullptr, nullptr, "System Allocator")
{
}

arena::arena(u8* ptr, usize capacity, u16 alignment) :
    m_arena(ptr),
    m_capacity(capacity),
    m_alignment(alignment)
{
}

u8* arena::allocate_buffer(usize size)
{
    size = align(size, this->m_alignment);
    ASSERT((this->m_used + size) <= this->m_capacity, "Arena does not have enough capacity left");

    this->m_used += size;
    return this->m_arena + (this->m_used - size);
}

}
