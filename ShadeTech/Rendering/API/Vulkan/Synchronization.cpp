#include "Synchronization.h"

#include <cassert>

#include "Helpers.h"

namespace SHD {
namespace Renderer {
namespace Vulkan {

Fence::Fence(Device& device)
{
    this->m_device_ref = &device;
    this->m_fence = this->CreateFence(device);
}

Fence::~Fence()
{
    if (this->m_fence != nullptr) {
        assert(this->m_device_ref != nullptr);
        vkDestroyFence(*this->m_device_ref, this->m_fence, nullptr);
    }
}

VkFence Fence::CreateFence(Device& device)
{
    const VkFenceCreateInfo fence_desc = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT,
    };

    VkFence fence = nullptr;
    VK_CALL(vkCreateFence(device, &fence_desc, nullptr, &fence), "Failed to create vulkan fence");
    return fence;
}

}
}
}
