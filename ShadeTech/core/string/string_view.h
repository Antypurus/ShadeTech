#pragma once

#include <core/types.h>

namespace shd {

struct StringView
{
public:
    const char* string = nullptr;
    const uint64 length = 0;

public:
    StringView() = default;
	StringView(const char* string);//requires size calculation
	StringView(const char* string, uint64 length);
};

}
