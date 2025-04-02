#include "core/string/string.h"

#include "core/memory.h"
#include "core/move.h"
#include "string_utils.h"

namespace SHD {
string::~string()
{
    if (this->str != nullptr) {
        free_memory(this->str);
        this->length = 0;
        this->capacity = 0;
    }
}

string::string(const char* string) :
    length(StringLenght(string))
{
    capacity = this->length;
    this->str = (char*)AllocateMemory(this->length);
    Copy((void*)string, this->length, (void*)this->str);
}

string::string(const char* string, usize length) :
    length(length),
    capacity(length)
{
    this->str = (char*)AllocateMemory(this->length);
    Copy((void*)string, this->length, (void*)this->str);
}

string::string(char*&& string, usize length) :
    str(string),
    length(length),
    capacity(length)
{
}

string string::operator+(const string& other) const
{
    const usize resulting_size = this->length + other.length + 1;
    char* resulting_str = (char*)AllocateMemory(resulting_size);
    Copy((void*)this->str, this->length, (void*)resulting_str);
    Copy((void*)other.str, other.length, (void*)resulting_str, this->length);
    return { move(resulting_str), resulting_size };
}

string string::operator+(const char* other) const
{
    const usize other_lenght = StringLenght(other);
    const usize resulting_size = this->length + other_lenght + 1;
    char* resulting_str = (char*)AllocateMemory(resulting_size);
    Copy((void*)this->str, this->length, (void*)resulting_str);
    Copy((void*)other, other_lenght, (void*)resulting_str, this->length);
    return { move(resulting_str), resulting_size };
}

}
