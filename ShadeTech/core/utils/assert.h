#pragma once

// #include <core/../../ThirdParty/cpptrace/include/cpptrace/cpptrace.hpp>
#include <core/platform.h>
#include <core/utils/log.h>

#define ASSERT(X, message, ...)                                                                                        \
    if (!(X)) {                                                                                                        \
        LOG_ERROR(message, __VA_ARGS__);                                                                               \
        TRIGGER_DEBUG_TRAP();                                                                                          \
        std::quick_exit(-1);                                                                                           \
    }
// cpptrace::generate_trace().print();
