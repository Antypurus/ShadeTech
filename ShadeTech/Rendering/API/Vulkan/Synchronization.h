#pragma once

#include <vulkan/vulkan_core.h>

#include "Device.h"

namespace SHD {
namespace Renderer {
namespace Vulkan {

class Fence
{
private:
    VkFence m_fence = nullptr;
    Device* m_device_ref = nullptr;

public:
    Fence(Device& device);
    ~Fence();

private:
    VkFence CreateFence(Device& device);
};

}
}
}
