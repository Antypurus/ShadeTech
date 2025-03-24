module;

#include "Types.h"

import core.memory;
import core.string.utils;
export module core.string;

namespace SHD {

struct String
{
    char* string = nullptr;
    usize length = 0;
    usize capacity = 0;

public:
    String() = default;

    String(const char* string) :
        length(StringLenght(string))
    {
        capacity = this->length;
        this->string = (char*)AllocateMemory(this->length);
        Copy((void*)string, this->length, (void*)this->string);
    }
};

}
