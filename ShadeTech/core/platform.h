#pragma once

#define SHD_PLATFORM_WINDOWS 0
#define SHD_PLATFORM_LINUX 0
#define SHD_PLATFORM_MACOS 0
#define SHD_PLATFORM_ANDROID 0
#define SHD_PLATFORM_IOS 0
#define SHD_PLATFORM_POSIX 0

#if defined(_WIN64) || defined(_WIN32)
    #undef SHD_PLATFORM_WINDOWS
    #define SHD_PLATFORM_WINDOWS 1
#elif defined(__ANDROID__)
    #undef SHD_PLATFORM_ANDROID
    #define SHD_PLATFORM_ANDROID 1
#elif defined(__linux__)
    #undef SHD_PLATFORM_LINUX
    #undef SHD_PLATFORM_POSIX
    #define SHD_PLATFORM_LINUX 1
    #define SHD_PLATFORM_POSIX 1
#elif defined(__APPLE__)
    #undef SHD_PLATFORM_POSIX
    #define SHD_PLATFORM_POSIX 1
    #ifdef TARGET_OS_MAC
        #undef SHD_PLATFORM_MACOS
        #define SHD_PLATFORM_MACOS 1
    #else
        #undef SHD_PLATFORM_IOS
        #define SHD_PLATFORM_IOS 1
    #endif
#endif
