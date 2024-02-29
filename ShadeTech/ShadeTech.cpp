#include <iostream>
#include <GLFW/glfw3.h>

#include "Rendering/API/Vulkan/VulkanAPI.h"

using namespace SHD;
using namespace SHD::Renderer;

int main()
{
    std::cout << "Welcome To the SDH Network Agent\n";

    Vulkan::Instance instance(Vulkan::Instance::GetSupportedLayers(), Vulkan::Instance::GetSupportedExtensions());

    return 0;
}
