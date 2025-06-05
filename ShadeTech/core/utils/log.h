#pragma once

#include <core/platform.h>

#include <print>

#define LOGGING_ENABLED 1

#if LOGGING_ENABLED

#define TO_STR(X) STR(X)
#define STR(X) #X

#define GREEN_FG "\033[0;32m"
#define RED_FG "\033[0;31m"
#define YELLOW_FG "\033[1;33m"
#define BLUE_FG "\033[1;36m"
#define RESET_FG "\033[0m"

#if COMPILER_MSVC
#define SHD_OPT_EXPAND(...) , __VA_ARGS__
#else
#define SHD_OPT_EXPAND(...) __VA_OPT__(, ) __VA_ARGS__
#endif

#define LOG_ERROR(message, ...)                                                                                        \
    std::print(RED_FG "[ERROR][" __FILE__ "@" TO_STR(__LINE__) "]" message "\n" RESET_FG SHD_OPT_EXPAND(__VA_ARGS__))

#define LOG_WARN(message, ...)                                                                                         \
    std::print(YELLOW_FG "[WARNING][" __FILE__ "@" TO_STR(__LINE__) "]" message "\n" RESET_FG SHD_OPT_EXPAND(__VA_ARGS__))

#define LOG_SUCCESS(message, ...)                                                                                      \
    std::print(GREEN_FG "[SUCCESS][" __FILE__ "@" TO_STR(__LINE__) "]" message "\n" RESET_FG SHD_OPT_EXPAND(__VA_ARGS__))

#define LOG_INFO(message, ...)                                                                                         \
    std::print(BLUE_FG "[INFO][" __FILE__ "@" TO_STR(__LINE__) "]" message "\n" RESET_FG SHD_OPT_EXPAND(__VA_ARGS__))

#else

#define LOG_ERROR(message, ...)
#define LOG_WARN(message, ...)
#define LOG_SUCCESS(message, ...)
#define LOG_INFO(message, ...)

#endif
