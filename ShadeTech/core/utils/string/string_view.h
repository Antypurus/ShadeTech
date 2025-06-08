#pragma once

#include <core/types.h>
#include <core/utils/pair.h>
#include <core/utils/string/string.h>

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

    SHD::string to_string() const;
    string_view substring(usize start = 0, usize end = 0) const;

    /* Obtains the index of the last occurrence of a given token (right split/partition)*/
    usize rsplit_index(char token) const;
    pair<string_view, string_view> rsplit(char token) const;
    string_view rsplit_head(char token) const;
    string_view rsplit_tail(char token) const;

    /* Obtains the index of the first occurrence of a given token (left split/partition)*/
    usize lsplit_index(char token) const;
    pair<string_view, string_view> lsplit(char token) const;

public:
    string_view(string_view&& other) = delete;
    string_view& operator=(const string_view& other) = delete;
    string_view& operator=(string_view&& other) = delete;
};

}
