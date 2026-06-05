#pragma once

#define SHD_COMPILER_MSVC 0
#define SHD_COMPILER_GCC 0
#define SHD_COMPILER_CLANG 0

#if defined(__GNUC__)
    #undef SHD_COMPILER_GCC
    #define SHD_COMPILER_GCC 1
#elif defined(__clang__)
    #undef SHD_COMPILER_CLANG
    #define SHD_COMPILER_CLANG 1
#elif defined(_MSC_VER)
    #undef SHD_COMPILER_MSVC
    #define SHD_COMPILER_MSVC 1
#endif
