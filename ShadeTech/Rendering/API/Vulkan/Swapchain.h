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
    VkImage m_framebuffer_resources[2] = { nullptr, nullptr };
    Device* m_device_ref = nullptr;

public:
    Swapchain(Device& device, Window& window);
    ~Swapchain();

    Swapchain(Swapchain&& other);
    Swapchain& operator=(Swapchain&& other);

    inline operator VkSwapchainKHR() { return this->m_swapchain; }

private:
    VkSwapchainKHR CreateSwapchain(Device& device, Window& window) const;
    void FetchSwapchainResources(Device& device);

public:
    // deleted member constructors/operators
    Swapchain() = delete;
    Swapchain(const Swapchain& other) = delete;
    Swapchain& operator=(const Swapchain& other) = delete;
};

}
}
}
