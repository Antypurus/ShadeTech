#pragma once

#include <core/types.h>
#include <core/utils/assert.h>
#include <core/utils/memory/memory.h>
#include <core/utils/move.h>

namespace SHD {

template<typename ReturnT, typename... ParamsT>
struct function;

template<typename return_T, typename... params_T>
struct function<return_T(params_T...)>
{
private:
    using function_call_adapter = return_T (*)(const function&, params_T...);
    function_call_adapter m_call_adapter = nullptr;
    allocator* m_allocator = nullptr;
    u8* m_function_storage = nullptr;
    usize m_capture_group_storage_size = 0;

public:
    function() = default;

    ~function()
    {
        if (this->m_allocator != nullptr && this->m_capture_group_storage_size > 0) {
            this->m_allocator->free(this->m_function_storage, this->m_capture_group_storage_size);
        }

        this->m_allocator = nullptr;
        this->m_call_adapter = nullptr;
        this->m_function_storage = nullptr;
        this->m_capture_group_storage_size = 0;
    }

    function(return_T (*func_ptr)(params_T...)) :
        m_function_storage((u8*)func_ptr),
        m_capture_group_storage_size(0)
    {
        this->m_call_adapter = [](const function& func, params_T... params) -> return_T {
            using callable_type = return_T (*)(params_T...);
            const callable_type callable = (callable_type)func.m_function_storage;
            return callable(move(params)...);
        };
    }

    template<typename callable_T>
    function(const callable_T callable, allocator* allocator = DEFAULT_ALLOCATOR) :
        m_allocator(allocator),
        m_capture_group_storage_size(sizeof(callable))
    {
        this->m_function_storage = this->m_allocator->allocate<u8>(this->m_capture_group_storage_size);
        copy((void*)&callable, this->m_capture_group_storage_size, (void*)this->m_function_storage);

        this->m_call_adapter = [](const function& func, params_T... params) -> return_T {
            const callable_T* callable = (callable_T*)func.m_function_storage;
            return (*callable)(move(params)...);
        };
    }

    function(const function& other) :
        m_call_adapter(other.m_call_adapter),
        m_allocator(other.m_allocator),
        m_capture_group_storage_size(other.m_capture_group_storage_size)
    {
        if (this->m_allocator != nullptr && this->m_capture_group_storage_size > 0) {
            this->m_function_storage = this->m_allocator->allocate<u8>(this->m_capture_group_storage_size);
            copy(other.m_function_storage, other.m_capture_group_storage_size, this->m_function_storage);
        } else {
            this->m_function_storage = other.m_function_storage;
        }
    }

    function& operator=(const function& other)
    {
        if (this == &other)
            return *this;

        this->~function();
        this->m_call_adapter = other.m_call_adapter;
        this->m_allocator = other.m_allocator;
        this->m_capture_group_storage_size = other.m_capture_group_storage_size;
        if (this->m_allocator != nullptr && this->m_capture_group_storage_size > 0) {
            this->m_function_storage = this->m_allocator->allocate<u8>(this->m_capture_group_storage_size);
            copy(other.m_function_storage, other.m_capture_group_storage_size, this->m_function_storage);
        } else {
            this->m_function_storage = other.m_function_storage;
        }

        return *this;
    }

    function(function&& other) :
        m_call_adapter(other.m_call_adapter),
        m_allocator(other.m_allocator),
        m_function_storage(other.m_function_storage),
        m_capture_group_storage_size(other.m_capture_group_storage_size)
    {
        other.m_call_adapter = nullptr;
        other.m_allocator = nullptr;
        other.m_function_storage = nullptr;
        other.m_capture_group_storage_size = 0;
    }

    function& operator=(function&& other)
    {
        if (this == &other)
            return *this;

        this->~function();
        this->m_call_adapter = other.m_call_adapter;
        this->m_allocator = other.m_allocator;
        this->m_function_storage = other.m_function_storage;
        this->m_capture_group_storage_size = other.m_capture_group_storage_size;
        other.m_call_adapter = nullptr;
        other.m_allocator = nullptr;
        other.m_function_storage = nullptr;
        other.m_capture_group_storage_size = 0;

        return *this;
    }

    return_T operator()(params_T... params)
    {
        ASSERT(this->m_call_adapter != nullptr, "Un-Initialized function calling is not permitted");
        return this->m_call_adapter(*this, move(params)...);
    }
};

}
