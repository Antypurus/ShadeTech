#pragma once

#include <core/types.h>

namespace shd {

class StringView;

class FatString
{
    char* string = nullptr;
    uint64 length = 0;
    uint64 capacity = 0;
};

class SmallString
{
    char string[sizeof(FatString)];
};

class String
{
    union
    {
        SmallString smallString = SmallString();
        FatString fatString;
    } string;

    String() = default;

    operator StringView() const;
};

}
