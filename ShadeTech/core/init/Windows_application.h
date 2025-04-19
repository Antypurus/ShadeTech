#pragma once

#include "core/utils/log.h"
#include "core/networking/platform/Windows/networking.h"

namespace SHD::Windows {

class application
{
public:
    application()
    {
        LOG_INFO("Initializing Windows Modules");
        Networking::init_networking_module();
    }
};

}
