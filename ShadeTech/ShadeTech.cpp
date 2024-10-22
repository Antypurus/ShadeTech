import core;

#include "Log.h"
#include "assert.h"

#if PLATFORM_WINDOWS
#include <Windows.h>
#include <dbghelp.h>
#else
#include <dlfcn.h>
#include <execinfo.h>
#include <libdwarf.h>
#include <dwarf.h>
#include <libunwind.h>
#include <fcntl.h>
#include <unistd.h>
#endif

using namespace SHD::Rendering::RHI::Vulkan;

int main()
{
    LOG_SUCCESS("Welcome To the SDH Network Agent");

#if PLATFORM_WINDOWS
    auto* process = GetCurrentProcess();
    auto* thread = GetCurrentThread();

    CONTEXT context;
    RtlCaptureContext(&context);

    STACKFRAME64 stackframe = {
        .AddrPC = {
            .Offset = context.Rip,
            .Mode = AddrModeFlat,
        },
        .AddrFrame = {
            .Offset = context.Rbp,
            .Mode = AddrModeFlat,
        },
        .AddrStack = {
            .Offset = context.Rsp,
            .Mode = AddrModeFlat,
        },
    };

    SymInitialize(process, NULL, TRUE);

    while (StackWalk64(IMAGE_FILE_MACHINE_AMD64,
                       process,
                       thread,
                       &stackframe,
                       &context,
                       nullptr,
                       SymFunctionTableAccess64,
                       SymGetModuleBase64,
                       nullptr)) {

        auto address = stackframe.AddrPC.Offset;
        if (address == 0)
            break;

        char symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
        auto* symbol = (SYMBOL_INFO*)(void*)symbolBuffer;
        symbol->MaxNameLen = MAX_SYM_NAME;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

        auto displacement = 0ULL;
        if (SymFromAddr(process, address, &displacement, symbol)) {
            LOG_INFO("Name:%s", (char*)symbol->Name);

            // line info
            IMAGEHLP_LINE64 line = {
                .SizeOfStruct = sizeof(IMAGEHLP_LINE64),
            };
            auto lineDisplacement = 0UL;
            if (SymGetLineFromAddr64(process, address, &lineDisplacement, &line)) {
                LOG_INFO("\t Line: %ld, File: %s", line.LineNumber, line.FileName);
            }
        } else {
            LOG_INFO("Missing Symbol At Address: %lld", address);
        }
    }

    SymCleanup(process);
#else
    unw_cursor_t cursor;
    unw_context_t context;

    unw_getcontext(&context);
    unw_init_local(&cursor, &context);

    do{
        unw_word_t ip, offset;
        unw_get_reg(&cursor, UNW_REG_IP, &ip);
        unw_get_proc_name(&cursor, nullptr, 0, &offset);


        int fd = open("/proc/self/exe", O_RDONLY);
        Dwarf_Debug dbg = nullptr;
        Dwarf_Error err;
        dwarf_init(fd, nullptr, nullptr, nullptr, &dbg, &err);

    }while (unw_step(&cursor) > 0);

#endif

    return 0;
}
