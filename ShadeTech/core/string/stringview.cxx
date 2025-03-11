module;

#include "Types.h"

export module core.stringview;

namespace SHD {

struct StringView
{
public:
    char* string = nullptr;
    u64 size = 0;
};

}
