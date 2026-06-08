#include <stdio.h>

#include <core/memory/arena.h>
#include <core/platform.h>

int main()
{
    auto arena = shd::memory::CreateArenaFromLibC(1024);
    printf("Hello world %d\n", SHD_PLATFORM_MACOS);
    return 0;
}
