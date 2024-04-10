#include "Synchronization.h"

#include <cassert>
#include <vulkan/vulkan_core.h>

#include "Helpers.h"
#include "Platform.h"

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

bool Fence::HasBeenSignaled() const
{
    VkResult result = vkGetFenceStatus(*this->m_device_ref, this->m_fence);
    switch (result) {
        case (VK_SUCCESS): {
            return true;
            break;
        }
        case (VK_NOT_READY): {
            return false;
            break;
        }
        default: {
            // error condition
            LOG_ERROR("Failed to fetch fence status. Error code = %d", (uint32)result);
#ifndef NDEBUG
            TRIGGER_DEBUG_TRAP();
            abort();
#endif
            return false;
            break;
        }
    }
}

bool Fence::WaitForSignal(uint64 timeout) const
{
    uint64 wait_time = timeout;
    if (timeout == 0) {
        // wait forever
        wait_time = ~0ULL;
    }

    VkResult result = vkWaitForFences(*this->m_device_ref, 1, &this->m_fence, VK_TRUE, wait_time);
    switch (result) {
        case (VK_SUCCESS): {
            return true;
            break;
        }
        case (VK_TIMEOUT): {
            return false;
            break;
        }
        default: {
            LOG_ERROR("Failed to wait for fence to be signaled. Error code = %d", (uint32)result);
#ifndef NDEBUG
            TRIGGER_DEBUG_TRAP();
            abort();
#endif
            return false;
            break;
        }
    }
}

}
}
}
