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

    inline operator VkFence&() { return this->m_fence; }

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

    PipelineEvent(PipelineEvent&& other);
    PipelineEvent& operator=(PipelineEvent&& other);

    inline operator VkEvent&() { return this->m_event; }

    bool HasBeenSignaled() const;
    void Signal();
    void Reset();

private:
    VkEvent CreatePipelineEvent(Device& device) const;

public:
    // deleted constructors & operators
    PipelineEvent(const PipelineEvent& other) = delete;
    PipelineEvent& operator=(const PipelineEvent& other) = delete;
};

class DeviceSemaphore
{
private:
    VkSemaphore m_semaphore = nullptr;
    Device* m_device_ref = nullptr;

public:
    DeviceSemaphore(Device& device);
    ~DeviceSemaphore();

    DeviceSemaphore(DeviceSemaphore&& other);
    DeviceSemaphore& operator=(DeviceSemaphore&& other);

    inline operator VkSemaphore&() { return this->m_semaphore; }

private:
    VkSemaphore CreateSemaphore(Device& device) const;

public:
    // deleted constructors & operators
    DeviceSemaphore(const DeviceSemaphore& other) = delete;
    DeviceSemaphore& operator=(const DeviceSemaphore& other) = delete;
};

}
}
}
