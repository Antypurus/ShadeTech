module;

#include "Types.h"
#include "core/move.h"

import core.memory;
import core.string.utils;
export module core.string;

/*
 * TODO: very crap impl to lay the ground for the API needs. Improve later
 *  - Add allocator support
 */

namespace SHD {

export struct string
{
    char* str = nullptr;
    usize length = 0;
    usize capacity = 0;

public:
    string() = default;

    ~string()
    {
        if(this->str != nullptr)
        {
            free_memory(this->str);
            this->length = 0;
            this->capacity = 0;
        }
    }

    string(const char* string) :
        length(StringLenght(string))
    {
        capacity = this->length;
        this->str = (char*)AllocateMemory(this->length);
        Copy((void*)string, this->length, (void*)this->str);
    }

    string(const char* string, usize length) :
        length(length),
        capacity(length)
    {
        this->str = (char*)AllocateMemory(this->length);
        Copy((void*)string, this->length, (void*)this->str);
    }

    string(char*&& string, usize length) :
        str(string),
        length(length),
        capacity(length)
    {
    }

    string operator+(const string& other) const
    {
        const usize resulting_size = this->length + other.length + 1;
        char* resulting_str = (char*)AllocateMemory(resulting_size);
        Copy((void*)this->str, this->length, (void*)resulting_str);
        Copy((void*)other.str, other.length, (void*)resulting_str, this->length);
        return { move(resulting_str), resulting_size };
    }

    string operator+(const char* other) const
    {
        const usize other_lenght = StringLenght(other);
        const usize resulting_size = this->length + other_lenght + 1;
        char* resulting_str = (char*)AllocateMemory(resulting_size);
        Copy((void*)this->str, this->length, (void*)resulting_str);
        Copy((void*)other, other_lenght, (void*)resulting_str, this->length);
        return { move(resulting_str), resulting_size };
    }
};

}
