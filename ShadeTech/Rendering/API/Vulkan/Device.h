#pragma once

#include "Rendering/API/Vulkan/Instance.h"
#include "Types.h"

namespace SHD {
namespace Renderer {
namespace Vulkan {

class Device;

enum class QueueType
{
    Graphics,
    Compute,
    Transfer,
    Unknown
};

class Queue
{
private:
    VkQueue m_queue = nullptr;
    QueueType m_type = QueueType::Unknown;

public:
    Queue() = default;
    Queue(VkQueue queue_handle);

};

class Device
{
private:
    PhysicalDeviceInfo* m_device_info_ref = nullptr;
    VkDevice m_device = nullptr;
    Queue m_command_queue = nullptr;

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
