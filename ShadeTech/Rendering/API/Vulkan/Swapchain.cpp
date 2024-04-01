#include "Swapchain.h"

#include "Rendering/API/Vulkan/Instance.h"

namespace SHD {
namespace Renderer {
namespace Vulkan {

Swapchain::Swapchain(Device& device, Window& window)
{
    this->m_surface = window.CreateVulkanSurface();
}

Swapchain::~Swapchain()
{
    if(this->m_surface != nullptr)
    {
        vkDestroySurfaceKHR(Instance::GetInstance(), this->m_surface, nullptr);
    }
}

}
}
}
