#include "string_view.h"

#include <core/utils/assert.h>
#include <core/utils/string/string_utils.h>

namespace SHD {

string_view::string_view(const char* str) :
    string(str),
    length(string_lenght(str))
{
}

string_view::string_view(const char* str, usize length) :
    string(str),
    length(length)
{
}

string_view::string_view(const string_view& other) :
    string(other.string),
    length(other.length)
{
}

string_view& string_view::operator=(const string_view& other)
{
    if (this == &other)
        return *this;

    this->string = other.string;
    this->length = other.length;

    return *this;
}

string_view& string_view::operator=(const char* str)
{
    if (this->string == str)
        return *this;

    this->string = str;
    this->length = string_lenght(str);

    return *this;
}

const char* string_view::begin()
{
    return this->string;
}

const char* string_view::end()
{
    return this->string + (this->length - 1);
}

char string_view::operator[](usize index) const
{
    ASSERT(index < this->length, "index must be withing string_view bounds");
    return this->string[index];
}

SHD::string string_view::to_string() const
{
    return SHD::string(this->string, this->length);
}

string_view string_view::substring(usize start, usize end) const
{
    return string_view{ this->string + start, end - start };
}

usize string_view::rsplit_index(char token) const
{
    return last_token_occurence(this->string, this->length, token);
}

pair<string_view, string_view> string_view::rsplit(char token) const
{
    // TODO(Tiago): check boundaries
    usize idx = this->rsplit_index(token);
    if (idx > 0) {
        return {
            { this->string, idx },                             // head
            { this->string + idx + 1, this->length - idx - 1 } // tail
        };
    } else {
        return {
            { this->string, 0 },           // head
            { this->string, this->length } // tail
        };
    }
}

string_view string_view::rsplit_head(char token) const
{
    usize idx = this->rsplit_index(token);
    if (idx > 0) {
        return string_view{ this->string, idx };
    } else {
        return string_view{ this->string, 0 };
    }
}

string_view string_view::rsplit_tail(char token) const
{
    usize idx = this->rsplit_index(token);
    if (idx > 0) {
        return string_view{ this->string + idx + 1, this->length - idx - 1 };
    } else {
        return string_view{ this->string, this->length };
    }
}

usize string_view::lsplit_index(char token) const
{
    return first_token_occurence(this->string, this->length, token);
}

}
