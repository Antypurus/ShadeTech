module;

#include "Log.h"
#include "Platform.h"

import windows.networking;
export module windows.application;

#if PLATFORM_WINDOWS
namespace SHD::Windows {
export class Application
{
public:
    Application()
    {
        LOG_INFO("Initializing Windows Modules");
        Networking::InitNetworkingModule();
    }
};
}

#endif
