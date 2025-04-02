#pragma once

#include "Log.h"
#include "Networking/Networking.h"
#include "Platform.h"

namespace SHD::Windows {
class Application
{
public:
    Application()
    {
        LOG_INFO("Initializing Windows Modules");
        Networking::InitNetworkingModule();
    }
};
}
