#include <iostream>

#include "Log.h"
#include "Rendering/API/Vulkan/Device.h"
#include "Rendering/API/Vulkan/Instance.h"
#include "Rendering/Window.h"

using namespace SHD;
using namespace SHD::Renderer;

int main()
{
    LOG_SUCCESS("Welcome To the SDH Network Agent");

    Window window("SHD-ISAC Terminal", 1280, 720);
    Vulkan::Device device;
    while (window.IsOpen()) {
        window.Update();
    }

    return 0;
}
