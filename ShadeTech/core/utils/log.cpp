#include "log.h"

#include <core/platform.h>

#include <format>
#include <print>
#include <stdarg.h>
#include <string>
#include <vector>

#pragma warning(push)
#pragma warning(disable : 4840)

namespace SHD {

#define GREEN_FG "\033[0;32m"
#define RED_FG "\033[0;31m"
#define YELLOW_FG "\033[1;33m"
#define BLUE_FG "\033[1;36m"
#define RESET_FG "\033[0m"

// TODO(Tiago): substitute all usages of std lib

// TODO(Tiago): SIMD speedup???
std::vector<usize> find_pattern_replacement_fields(string_view pattern)
{
    std::vector<usize> result;
    for (usize i = 0; i < pattern.length; ++i) {
        if (i + 1 < pattern.length) {
            if (pattern[i] == '{' && pattern[i + 1] == '}') {
                // found one add it to vector
                result.push_back(i);
            }
        }
    }
    return result;
}

void print(string_view file, usize line, log_level level, string_view message, ...)
{
    string_view foreground_ansi_code = "";
    string_view reset_foregrounf_ansi_core = RESET_FG;
    string_view log_level = "";
    switch (level) {
        case (log_level::error): {
            log_level = "ERROR";
            foreground_ansi_code = RED_FG;
            break;
        }
        case (log_level::warning): {
            log_level = "WARNING";
            foreground_ansi_code = YELLOW_FG;
            break;
        }
        case (log_level::info): {
            log_level = "INFO";
            foreground_ansi_code = BLUE_FG;
            break;
        }
        case (log_level::success): {
            log_level = "SUCCESS";
            foreground_ansi_code = GREEN_FG;
            break;
        }
        default: {
            break;
        }
    }

#if COMPILER_MSVC
    char rsplit_token = '\\';
#else
    char rsplit_token = '/';
#endif
    SHD::string shortedned_filepath = file.rsplit_tail(rsplit_token).to_string();

    va_list args;
    va_start(args, message);

    std::string arg_string = std::vformat(message.string, std::make_format_args(args));
    std::print("{}[{}][{}@{}]:{}\n{}",
               foreground_ansi_code.string,
               log_level.string,
               shortedned_filepath.str,
               line,
               arg_string,
               reset_foregrounf_ansi_core.string);

    va_end(args);
}
}
