#pragma once

#include <stdlib.h>
#include <Platform.h>

#define _SHD_CONCAT(X,Y) X ## Y
#define SHD_CONCAT(X,Y) _SHD_CONCAT(X,Y)

#define VK_CALL(X) \
    const VkResult SHD_CONCAT(result, __LINE__) = X;\
    if(SHD_CONCAT(result, __LINE__) != VK_SUCCESS) { \
        TRIGGER_DEBUG_TRAP(); \
        abort(); \
    }
