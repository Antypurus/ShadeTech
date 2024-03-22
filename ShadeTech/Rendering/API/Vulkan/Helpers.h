#pragma once

#include <Log.h>
#include <Platform.h>
#include <stdlib.h>

#define VK_CALL(OP, ERROR_MESSAGE)                                                                                     \
    {                                                                                                                  \
        const VkResult SHD_CONCAT(result, __LINE__) = OP;                                                              \
        if (SHD_CONCAT(result, __LINE__) != VK_SUCCESS) {                                                              \
            LOG_ERROR(ERROR_MESSAGE);                                                                                  \
            TRIGGER_DEBUG_TRAP();                                                                                      \
            abort();                                                                                                   \
        }                                                                                                              \
    }
