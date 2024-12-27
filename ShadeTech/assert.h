#pragma once

#include "Log.h"
#include "Platform.h"
#include <cpptrace/cpptrace.hpp>

#define ASSERT(X, message, ...)                                                                                        \
    if (!(X)) {                                                                                                        \
        LOG_ERROR(message, __VA_ARGS__);                                                                               \
        cpptrace::generate_trace().print();                                                                            \
        TRIGGER_DEBUG_TRAP();                                                                                          \
        exit(-1);                                                                                                      \
    }
