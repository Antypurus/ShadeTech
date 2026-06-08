#include <stdio.h>

#include <core/memory/arena.h>
#include <core/platform.h>

#include <stdlib.h>

int main()
{
    auto arena = shd::Arena::CreateFromLibC(1024);
    free(arena.resultUnion.value.base);
    printf("Hello world %d\n", SHD_PLATFORM_MACOS);
    return 0;
}
