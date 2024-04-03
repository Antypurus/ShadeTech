#include "Swapchain.h"

#include <vulkan/vulkan_core.h>

#include "Helpers.h"
#include "Rendering/API/Vulkan/Instance.h"

namespace SHD {
namespace Renderer {
namespace Vulkan {

Swapchain::Swapchain(Device& device, Window& window)
{
    this->m_surface = window.CreateVulkanSurface();
    this->CreateSwapchain(device, window);
}

VkSwapchainKHR Swapchain::CreateSwapchain(Device& device, Window& window) const
{
    VkSwapchainCreateInfoKHR swapchain_description = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .surface = this->m_surface,
        .minImageCount = 2,                     // double buffered
        .imageFormat = VK_FORMAT_R8G8B8A8_SRGB, // hard coded for now
        .imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
        .imageExtent{
            .width = window.width,
            .height = window.height,
        },
    };

    VkSwapchainKHR swapchain = nullptr;
    VK_CALL(vkCreateSwapchainKHR(device, &swapchain_description, nullptr, &swapchain), "Failed to create swapchain");
    return nullptr;
}

Swapchain::~Swapchain()
{
    if (this->m_surface != nullptr) {
        vkDestroySurfaceKHR(Instance::GetInstance(), this->m_surface, nullptr);
    }
}

}
}
}
