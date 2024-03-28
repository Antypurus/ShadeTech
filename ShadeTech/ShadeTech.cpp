#include <iostream>

#include "GLFW/glfw3.h"
#include "Log.h"
#include "Rendering/API/Vulkan/Device.h"

using namespace SHD;
using namespace SHD::Renderer;

int main()
{
    glfwInit();
    LOG_SUCCESS("Welcome To the SDH Network Agent");

    Vulkan::Device device;

    return 0;
}
