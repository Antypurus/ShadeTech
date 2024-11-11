import core;

#include "Log.h"
#include "assert.h"

#include <cpptrace/cpptrace.hpp>

using namespace SHD::Rendering::RHI::Vulkan;

int main(int argc, char** argv)
{
    LOG_SUCCESS("Welcome To the SDH Network Agent");
    cpptrace::generate_trace().print();

    return 0;
}
