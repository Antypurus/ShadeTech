#pragma once

#define INNER_SHD_CONCAT(X, Y) X##Y
#define SHD_CONCAT(X, Y) INNER_SHD_CONCAT(X, Y)

#ifdef _WIN32
#define PLATFORM_WINDOWS 1
#define PLATFORM_POSIX 0
#define PLATFORM_MACOS 0
#define PLATFORM_LINUX 0
#endif

#ifdef __APPLE__
#define PLATFORM_MACOS 1
#define PLATFORM_POSIX 1
#define PLATFORM_LINUX 0
#define PLATFORM_WINDOWS 0
#endif

#ifdef __linux__
#define PLATFORM_LINUX 1
#define PLATFORM_POSIX 1
#define PLATFORM_MACOS 0
#define PLATFORM_WINDOWS 0
#endif

#ifndef NDEBUG
#if PLATFORM_WINDOWS
#define TRIGGER_DEBUG_TRAP() __debugbreak()
#elif PLATFORM_POSIX
#include <signal.h>
#define TRIGGER_DEBUG_TRAP() (void)raise(SIGTRAP)
#else
#define TRIGGER_DEBUG_TRAP()
#endif
#else
#define TRIGGER_DEBUG_TRAP()
#endif

#ifdef _MSC_VER

#ifdef __clang__
#define COMPILER_GCC 0
#define COMPILER_MSVC 0
#define COMPILER_CLANG 1
#else
#define COMPILER_MSVC 1
#define COMPILER_CLANG 0
#define COMPILER_GCC 0
#endif

#elif defined(__clang__)

#define COMPILER_CLANG 1
#define COMPILER_MSVC 0
#define COMPILER_GCC 0

#elif defined(__GNUC__)

#define COMPILER_GCC 1
#define COMPILER_MSVC 0
#define COMPILER_CLANG 0

#endif
