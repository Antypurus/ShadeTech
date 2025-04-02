#pragma once

#include "log.h"
#include "platform.h"
#include <cpptrace/cpptrace.hpp>

#define ASSERT(X, message, ...)                                                                                        \
    if (!(X)) {                                                                                                        \
        LOG_ERROR(message, __VA_ARGS__);                                                                               \
        cpptrace::generate_trace().print();                                                                            \
        TRIGGER_DEBUG_TRAP();                                                                                          \
        std::quick_exit(-1);                                                                                           \
    }
