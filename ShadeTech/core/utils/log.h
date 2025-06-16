#pragma once

#include <core/platform.h>
#include <core/utils/string/string_view.h>

#include <vector>

namespace SHD {

enum class log_level
{
    error,
    warning,
    info,
    success
};

std::vector<usize> find_pattern_replacement_fields(string_view pattern);
void print(string_view file, usize line, log_level level, string_view message, ...);

}

#define LOGGING_ENABLED 1

#if LOGGING_ENABLED

#define TO_STR(X) STR(X)
#define STR(X) #X

#if COMPILER_MSVC
#define SHD_OPT_EXPAND(...) , __VA_ARGS__
#else
#define SHD_OPT_EXPAND(...) __VA_OPT__(, ) __VA_ARGS__
#endif

//[ERROR_LEVEL][file@line]: message \n"
#define LOG_ERROR(message, ...) SHD::print(__FILE__, __LINE__, SHD::log_level::error, message, __VA_ARGS__)
#define LOG_WARN(message, ...) SHD::print(__FILE__, __LINE__, SHD::log_level::warning, message, __VA_ARGS__)
#define LOG_SUCCESS(message, ...) SHD::print(__FILE__, __LINE__, SHD::log_level::success, message, __VA_ARGS__)
#define LOG_INFO(message, ...) SHD::print(__FILE__, __LINE__, SHD::log_level::info, message, __VA_ARGS__)

#else

#define LOG_ERROR(message, ...)
#define LOG_WARN(message, ...)
#define LOG_SUCCESS(message, ...)
#define LOG_INFO(message, ...)

#endif
