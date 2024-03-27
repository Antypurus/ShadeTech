#include <iostream>

#include "Log.h"
#include "Rendering/API/Vulkan/Instance.h"

using namespace SHD;
using namespace SHD::Renderer;

int main()
{
    LOG_SUCCESS("Welcome To the SDH Network Agent");

    Vulkan::Instance::GetInstance().devices[0].LogDeviceInformation();

    return 0;
}
