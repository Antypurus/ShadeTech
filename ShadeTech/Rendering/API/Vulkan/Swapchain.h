#pragma once

#include <vulkan/vulkan_core.h>

#include "Device.h"
#include "Rendering/Window.h"

namespace SHD {
namespace Renderer {
namespace Vulkan {

class Swapchain
{
private:
    VkSurfaceKHR m_surface = nullptr;
    VkSwapchainKHR m_swapchain = nullptr;

public:
    Swapchain(Device& device, Window& window);
    ~Swapchain();

private:
    VkSwapchainKHR CreateSwapchain(Device& device, Window& window) const;
};

}
}
}
