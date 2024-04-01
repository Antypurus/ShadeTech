#include "Swapchain.h"

#include "Helpers.h"
#include "Rendering/API/Vulkan/Instance.h"

namespace SHD {
namespace Renderer {
namespace Vulkan {

Swapchain::Swapchain(Device& device, Window& window)
{
    this->m_surface = window.CreateVulkanSurface();
}

VkSwapchainKHR Swapchain::CreateSwapchain(Device& device) const
{
    VkSwapchainCreateInfoKHR swapchain_description = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
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
