#include <iostream>

#include "Log.h"
#include "Rendering/API/Vulkan/Instance.h"

using namespace SHD;
using namespace SHD::Renderer;

int main()
{
    std::cout << "Welcome To the SDH Network Agent\n";

    std::vector<std::string> extensions = Vulkan::Instance::GetSupportedExtensions();
    for(auto& extension: extensions)
    {
        LOG_INFO("%s", extension.c_str());
    }

    Vulkan::Instance instance;

    instance.devices[0].LogDeviceInformation();
    //instance.devices[0].CreateDevice();

    return 0;
}
