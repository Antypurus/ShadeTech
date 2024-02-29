#pragma once

#include <stdlib.h>

#define _CONCAT(X,Y) X ## Y
#define CONCAT(X,Y) _CONCAT(X,Y)

#define VK_CALL(X) \
    const VkResult CONCAT(result, __LINE__) = X;\
    if(CONCAT(result, __LINE__) != VK_SUCCESS) { \
        abort(); \
    }
