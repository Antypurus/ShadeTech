#pragma once

#include <core/types.h>
#include <core/utils/memory/memory.h>

/*
 * TODO: very crap impl to lay the ground for the API needs. Improve later
 *  - Add allocator support
 */

namespace SHD {

struct string
{
private:
    allocator* m_allocator = nullptr;

public:
    char* str = nullptr;
    usize length = 0;
    usize capacity = 0;

public:
    string() = default;
    ~string();

    string(const char* string, allocator* allocator = &system_allocator::instance());
    string(const char* string, usize length, allocator* allocator = &system_allocator::instance());
    string(char*&& string, usize length, allocator* source_allocator);

    string(const string& other);
    string(string&& other);
    string& operator=(const string& other);
    string& operator=(string&& other);

    string operator+(const string& other) const;
    string operator+(const char* other) const;
};

}
