#pragma once

#include "types.h"

namespace SHD {

struct StringView
{
public:
    char* string = nullptr;
    u64 size = 0;
};

}
