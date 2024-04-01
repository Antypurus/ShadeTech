#pragma once

#include "Rendering/API/Vulkan/Instance.h"
#include "Types.h"

namespace SHD {
namespace Renderer {
namespace Vulkan {

class Device
{
private:
    PhysicalDeviceInfo* m_device_info_ref = nullptr;
    VkDevice m_device = nullptr;
    VkQueue m_command_queue = nullptr;

public:
    Device();
    Device(uint32 device_index);
    ~Device();

    operator VkDevice&() { return this->m_device; }

private:
    VkDevice CreateVKDevice(uint32 device_index) const;
    VkQueue CreateCommandQueue(uint32 family_index, uint32 queue_index) const;
};

}
}
}
