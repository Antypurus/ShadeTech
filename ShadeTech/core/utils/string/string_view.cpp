#include "string_view.h"

#include "string_utils.h"

namespace SHD
{

string_view::string_view(const char* str):
    string(str),
    length(string_lenght(str))
{
}

string_view::string_view(const char* str, usize length):
    string(str),
    length(length)
{
}

string_view::string_view(const string_view& other):
    string(other.string),
    length(other.length)
{
}

}