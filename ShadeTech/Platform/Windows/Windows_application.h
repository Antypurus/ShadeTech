#pragma once

#include "Networking/networking.h"
#include "log.h"

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
