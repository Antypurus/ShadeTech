#include "ShadeTech.cpp"
#include <core/types.h>

int main(int argc, char** argv)
{
    return shade_tech::run_application(shade_tech{}, (u32)argc, argv);
}
