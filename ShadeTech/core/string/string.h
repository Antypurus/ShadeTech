#pragma once

#include "Types.h"

/*
 * TODO: very crap impl to lay the ground for the API needs. Improve later
 *  - Add allocator support
 */

namespace SHD {

struct string
{
    char* str = nullptr;
    usize length = 0;
    usize capacity = 0;

public:
    string() = default;
    ~string();
    string(const char* string);
    string(const char* string, usize length);
    string(char*&& string, usize length);
    string operator+(const string& other) const;
    string operator+(const char* other) const;
};

}
