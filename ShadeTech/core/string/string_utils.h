#pragma once

#include <core/string/string.h>

namespace shd {

template<typename T, typename U>
shd::String concatenate(T&& first, U&& second);

}
