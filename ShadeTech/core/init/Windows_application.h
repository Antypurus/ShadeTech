#pragma once

#include "core/networking/platform/Windows/networking.h"
#include "core/utils/log.h"

namespace SHD::Windows {

class application
{
public:
    application();
    virtual ~application();

private:
    void init_com();
    void deinit_com();
};

}
