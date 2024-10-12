#pragma once

#include "Log.h"
#include "Platform.h"

#define ASSERT(X, message, ...)                                                                                        \
    if (!(X)) {                                                                                                          \
        LOG_ERROR(message, __VA_ARGS__);                                                                               \
        TRIGGER_DEBUG_TRAP();                                                                                          \
        exit(-1); \
    }
