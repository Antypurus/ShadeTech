#pragma once

using u8 = unsigned char;
using uint8 = unsigned char;
using i8 = signed char;
using int8 = signed char;

using u16 = unsigned short;
using uint16 = unsigned short;
using i16 = signed short;
using int16 = signed short;

using u32 = unsigned int;
using uint32 = unsigned int;
using i32 = signed int;
using int32 = signed int;

using u64 = unsigned long long;
using uint64 = unsigned long long;
using i64 = signed long long;
using int64 = signed long long;

//TODO(Tiago): need a proper system to
//determine which size to give these
//arch-dependent types
using usize = u64;
using isize = i64;
