#pragma once

#include <Log.h>
#include <Platform.h>
#include <stdlib.h>

#define KiB (1024llu)
#define MiB (KiB * KiB)
#define GiB (MiB * KiB)

#define HAS_BITFLAG(value, flag) ((value) & (flag)) == (flag) ? true : false

#define VK_CALL(OP, ERROR_MESSAGE)                                                                                     \
    {                                                                                                                  \
        const VkResult SHD_CONCAT(result, __LINE__) = OP;                                                              \
        if (SHD_CONCAT(result, __LINE__) != VK_SUCCESS) {                                                              \
            LOG_ERROR("Error Code: %lld", (int64)SHD_CONCAT(result, __LINE__));                                        \
            LOG_ERROR(ERROR_MESSAGE);                                                                                  \
            TRIGGER_DEBUG_TRAP();                                                                                      \
            abort();                                                                                                   \
        }                                                                                                              \
    }
