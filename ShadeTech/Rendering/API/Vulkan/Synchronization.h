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

    operator VkFence&() { return this->m_fence; }

    bool HasBeenSignaled() const;
    bool WaitForSignal(uint64 timeout = 0) const;
    void Reset();

private:
    VkFence CreateFence(Device& device) const;
};

class PipelineEvent
{
private:
    VkEvent m_event = nullptr;
    Device* m_device_ref = nullptr;

public:
    PipelineEvent(Device& device);
    ~PipelineEvent();

    operator VkEvent&() { return this->m_event; }

    bool HasBeenSignaled() const;
    void Signal();
    void Reset();

private:
    VkEvent CreatePipelineEvent(Device& device) const;
};

}
}
}
