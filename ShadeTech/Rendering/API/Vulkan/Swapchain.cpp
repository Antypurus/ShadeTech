#include "Swapchain.h"

#include <vulkan/vulkan_core.h>

#include "Helpers.h"
#include "Rendering/API/Vulkan/Instance.h"
#include "Types.h"

namespace SHD {
namespace Renderer {
namespace Vulkan {

Swapchain::Swapchain(Device& device, Window& window) :
    m_device_ref(&device)
{
    this->m_surface = window.CreateVulkanSurface();
    this->m_swapchain = this->CreateSwapchain(device, window);
}

Swapchain::~Swapchain()
{
    if (this->m_swapchain != nullptr) {
        vkDestroySwapchainKHR(*this->m_device_ref, this->m_swapchain, nullptr);
    }
    if (this->m_surface != nullptr) {
        vkDestroySurfaceKHR(Instance::GetInstance(), this->m_surface, nullptr);
    }
}

VkSwapchainKHR Swapchain::CreateSwapchain(Device& device, Window& window) const
{
    const uint32 queues[] = {
        0,
    };

    VkSwapchainCreateInfoKHR swapchain_description = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .surface = this->m_surface,
        .minImageCount = 2,                     // double buffered
        .imageFormat = VK_FORMAT_B8G8R8A8_SRGB, // hard coded for now
        .imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
        .imageExtent{
            .width = window.width,
            .height = window.height,
        },
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 1,
        .pQueueFamilyIndices = (const uint32*)queues,
        .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = VK_PRESENT_MODE_FIFO_KHR,
        .clipped = VK_TRUE,
        .oldSwapchain = this->m_swapchain,
    };

    VkSwapchainKHR swapchain = nullptr;
    VK_CALL(vkCreateSwapchainKHR(device, &swapchain_description, nullptr, &swapchain), "Failed to create swapchain");
    return swapchain;
}

Swapchain::Swapchain(Swapchain&& other)
{
    this->m_surface = other.m_surface;
    other.m_surface = nullptr;

    this->m_swapchain = other.m_swapchain;
    other.m_swapchain = nullptr;

    this->m_device_ref = other.m_device_ref;
    other.m_device_ref = nullptr;
}

Swapchain& Swapchain::operator=(Swapchain&& other)
{
    if (this == &other)
        return *this;

    this->m_surface = other.m_surface;
    other.m_surface = nullptr;

    this->m_swapchain = other.m_swapchain;
    other.m_swapchain = nullptr;

    this->m_device_ref = other.m_device_ref;
    other.m_device_ref = nullptr;

    return *this;
}

}
}
}
