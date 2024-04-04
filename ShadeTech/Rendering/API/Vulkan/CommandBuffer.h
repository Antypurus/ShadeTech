#pragma once

#include <vulkan/vulkan_core.h>

#include "Device.h"
#include <Types.h>

namespace SHD {
namespace Renderer {
namespace Vulkan {

class CommandPool
{
private:
    VkCommandPool m_command_poll = nullptr;

public:
    CommandPool(Device& device, uint8 queue_index);

private:
    VkCommandPool CreateCommandPool(Device& device, uint8 queue_index);
};

}
}
}
