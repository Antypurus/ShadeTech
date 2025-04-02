#pragma once

#include "Types.h"

namespace SHD {

struct StringView
{
public:
    char* string = nullptr;
    u64 size = 0;
};

}
