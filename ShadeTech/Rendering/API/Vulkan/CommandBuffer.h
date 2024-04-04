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
    Device* m_device_ref = nullptr;

public:
    CommandPool() = default;
    CommandPool(Device& device, uint8 queue_index);
    ~CommandPool();

    CommandPool(CommandPool&& other);
    CommandPool& operator=(CommandPool&& other);

private:
    VkCommandPool CreateCommandPool(Device& device, uint8 queue_index);

public:
    //deleted constructors & operators
    CommandPool(const CommandPool& other) = delete;
    CommandPool& operator=(const CommandPool& other) = delete;

};

}
}
}
