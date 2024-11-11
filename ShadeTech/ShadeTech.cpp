import core;

#include "Log.h"
#include "assert.h"

#if PLATFORM_WINDOWS
#include <Windows.h>
#include <dbghelp.h>
#else
#include <dlfcn.h>
#include <dwarf.h>
#include <execinfo.h>
#include <fcntl.h>
#include <libdwarf.h>
#include <libunwind.h>
#include <unistd.h>
#endif

#include <cpptrace/cpptrace.hpp>

using namespace SHD::Rendering::RHI::Vulkan;

int main(int argc, char** argv)
{
    LOG_SUCCESS("Welcome To the SDH Network Agent");
    cpptrace::generate_trace().print();

    return 0;
}
