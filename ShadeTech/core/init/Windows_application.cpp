#include "Windows_application.h"

#include <Windows.h>

namespace SHD::Windows {

application::application()
{
    //SetConsoleOutputCP(CP_UTF8); // CP_UTF8
    LOG_INFO("Initializing Windows Modules");
    this->init_com();
    Networking::init_networking_module();
}

application::~application()
{
    this->deinit_com();
}

void application::init_com()
{
    HRESULT result = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (result != S_OK) {
        LOG_ERROR("Failed to initialized COM library");
    }
}

void application::deinit_com()
{
    CoUninitialize();
}

}
