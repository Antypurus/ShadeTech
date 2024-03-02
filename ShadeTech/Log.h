#include <stdio.h>

#define LOGGING_ENABLED 1

#if LOGGING_ENABLED

#define TO_STR(X) STR(X)
#define STR(X) #X

#define GREEN_FG "\033[0;32m"
#define RED_FG "\033[0;31m"
#define YELLOW_FG "\033[1;33m"
#define WHITE_FG "\033[1;37m"
#define RESET_FG "\033[0m"

// MSVC doesnt have __VA_OPT__, however, it understand that if a command
// is followed by an empty __VA_ARGS__ then it should just remove the comma
// before doing syntax correctness checking, so to handle this we have
// to define our own wrapper around __VA_OPT__ to handle these diferences
// between compilers
#ifdef _MSC_VER
#define SHD_OPT(...) ,
#else
#define SHD_OPT(...) __VA_OPT__(__VA_ARGS__)
#endif

#define LOG_ERROR(message, ...)                                                                                        \
    printf(RED_FG "[ERROR][" __FILE__ "@" TO_STR(__LINE__) "]" message "\n" RESET_FG SHD_OPT(, ) __VA_ARGS__)

#define LOG_WARN(message, ...)                                                                                         \
    printf(YELLOW_FG "[WARNING][" __FILE__ "@" TO_STR(__LINE__) "]" message "\n" RESET_FG SHD_OPT(, ) __VA_ARGS__)

#define LOG_SUCCESS(message, ...)                                                                                      \
    printf(GREEN_FG "[SUCCESS][" __FILE__ "@" TO_STR(__LINE__) "]" message "\n" RESET_FG SHD_OPT(, ) __VA_ARGS__)

#define LOG_INFO(message, ...)                                                                                         \
    printf(WHITE_FG "[INFO][" __FILE__ "@" TO_STR(__LINE__) "]" message "\n" RESET_FG SHD_OPT(, ) __VA_ARGS__)

#else

#define LOG_ERROR(message, ...)
#define LOG_WARN(message, ...)
#define LOG_SUCCESS(message, ...)
#define LOG_INFO(message, ...)

#endif
