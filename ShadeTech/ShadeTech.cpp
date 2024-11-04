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

#if PLATFORM_LINUX
    std::string path = "/proc/self/exe";
#else
    std::string path = std::string(argv[0]) + ".dSYM/Contents/Resources/DWARF/ShadeTech";
#endif
    int fd = open(path.c_str(), O_RDONLY | O_CLOEXEC);
    if (fd < 0) {
        std::cout << "Failed to open process file" << std::endl;
    }

    Dwarf_Debug dbg = nullptr;
    Dwarf_Error err;
    auto ret = dwarf_init_b(fd, DW_GROUPNUMBER_ANY, nullptr, nullptr, &dbg, &err);
    if (ret == DW_DLV_ERROR) {
        std::cout << "failed to init libdwarf based on current process:" << dwarf_errmsg(err) << std::endl;
    } else if (ret == DW_DLV_NO_ENTRY) {
        std::cout << "no entry error" << std::endl;
    } else {
        LOG_SUCCESS("libdwarf initialized");
    }

    Dwarf_Unsigned next_cu_header;
    Dwarf_Half next_cu_type;
    ret = dwarf_next_cu_header_d(dbg,
                                 true,
                                 nullptr,
                                 nullptr,
                                 nullptr,
                                 nullptr,
                                 nullptr,
                                 nullptr,
                                 nullptr,
                                 nullptr,
                                 &next_cu_header,
                                 &next_cu_type,
                                 &err);
    if (ret != DW_DLV_OK) {
        LOG_ERROR("Failed to get cu header debug data");
    } else {
        LOG_INFO("GOT CU Data");
    }

    Dwarf_Die sibling = nullptr;
    do {
        ret = dwarf_siblingof_b(dbg, sibling, true, &sibling, &err);
        if (ret != DW_DLV_OK) {
            LOG_ERROR("Failed to get sibling");
            break;
        } else {
            LOG_INFO("Sibling Acquired");
        }
    } while (sibling != nullptr);

    unw_cursor_t cursor;
    unw_context_t context;
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);
    do {
        unw_word_t ip = 0, offset = 0;
        unw_get_reg(&cursor, UNW_REG_IP, &ip);
        unw_get_proc_name(&cursor, nullptr, 0, &offset);
    } while (unw_step(&cursor) > 0);

    // de-init dwarf
    dwarf_finish(dbg);
    close(fd);
#endif

    cpptrace::generate_trace().print();

    return 0;
}
