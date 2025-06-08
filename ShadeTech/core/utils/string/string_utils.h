#pragma once

#include <core/types.h>

namespace SHD {

usize string_lenght(const char* str);
usize last_token_occurence(const char* str, usize strlen, char token);
usize first_token_occurence(const char* str, usize strlen, char token);

}
