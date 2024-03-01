#pragma once

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
#define TRIGGER_DEBUG_TRAP() raise(SIGTRAP)
#else
#define TRIGGER_DEBUG_TRAP()
#endif

#else

#define TRIGGER_DEBUG_TRAP()

#endif
