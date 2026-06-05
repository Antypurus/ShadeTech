#pragma once

#include <core/platform.h>

using uint8 = unsigned char;
using int8 = signed char;
static_assert(sizeof(uint8) == 1);
static_assert(sizeof(int8) == 1);

using uint16 = unsigned short;
using int16 = signed short;
static_assert(sizeof(uint16) == 2);
static_assert(sizeof(int16) == 2);

using uint32 = unsigned int;
using int32 = signed int;
static_assert(sizeof(uint32) == 4);
static_assert(sizeof(int32) == 4);

#if SHD_PLATFORM_POSIX
using uint64 = unsigned long;
using int64 = signed long;
#else
using uint64 = unsigned long long;
using int64 = signed long long;
#endif
static_assert(sizeof(uint64) == 8);
static_assert(sizeof(int64) == 8);
