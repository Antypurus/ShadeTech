#include <iostream>

#include "Log.h"
#include "Rendering/API/Vulkan/Instance.h"

using namespace SHD;
using namespace SHD::Renderer;

int main()
{
    std::cout << "Welcome To the SDH Network Agent\n";

    Vulkan::Instance instance;

    auto layers = instance.GetSupportedLayers();
    for (auto& layer : layers) {
        LOG_INFO("%s", layer.c_str());
    }

    auto extensions = instance.GetSupportedExtensions();
    for (auto& extension : extensions) {
        LOG_INFO("%s", extension.c_str());
    }

    // instance.devices[0].LogDeviceInformation();
    instance.devices[0].CreateDevice();

    return 0;
}
