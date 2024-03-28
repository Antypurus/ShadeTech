#pragma once

#include "Types.h"
#include "VulkanAPI.h"

namespace SHD {
namespace Renderer {
namespace Vulkan {

class Device
{
private:
    VkDevice m_device = nullptr;
    VkQueue m_command_queue = nullptr;

public:
    Device();
    Device(uint32 device_index);
    ~Device();

private:
    VkDevice CreateVKDevice(uint32 device_index) const;
    VkQueue CreateCommandQueue(uint32 family_index, uint32 queue_index) const;
};

}
}
}
