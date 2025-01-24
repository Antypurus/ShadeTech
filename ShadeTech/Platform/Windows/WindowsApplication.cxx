module;

#include "Log.h"
#include "Platform.h"

import windows.networking;
export module windows.application;

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
