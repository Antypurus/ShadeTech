#pragma once

#include <core/types.h>

namespace SHD {

struct string_view
{
public:
    const char* string = nullptr;
    const usize length = 0;

public:
    string_view() = default;
    string_view(const char* str);
    string_view(const char* str, usize length);
    string_view(const string_view& other);

public:
    string_view(string_view&& other) = delete;
    string_view& operator=(const string_view& other) = delete;
    string_view& operator=(string_view&& other) = delete;
};

}
