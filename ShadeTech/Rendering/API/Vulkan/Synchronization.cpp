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

VkFence Fence::CreateFence(Device& device) const
{
    const VkFenceCreateInfo fence_desc = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
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
#else
            return false;
#endif
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
#else
            return false;
#endif
            break;
        }
    }
}

void Fence::Reset()
{
    VK_CALL(vkResetFences(*this->m_device_ref, 1, &this->m_fence), "Failed to reset fence");
}

PipelineEvent::PipelineEvent(Device& device)
{
    this->m_device_ref = &device;
    this->m_event = this->CreatePipelineEvent(device);
}

PipelineEvent::~PipelineEvent()
{
    if (this->m_event != nullptr) {
        assert(this->m_device_ref != nullptr);
        vkDestroyEvent(*this->m_device_ref, this->m_event, nullptr);
    }
}

PipelineEvent::PipelineEvent(PipelineEvent&& other)
{
    if (this == &other)
        return;

    this->m_event = other.m_event;
    this->m_device_ref = other.m_device_ref;

    other.m_event = nullptr;
    other.m_device_ref = nullptr;
}

PipelineEvent& PipelineEvent::operator=(PipelineEvent&& other)
{
    if (this == &other)
        return *this;

    this->m_event = other.m_event;
    this->m_device_ref = other.m_device_ref;

    other.m_event = nullptr;
    other.m_device_ref = nullptr;

    return *this;
}

VkEvent PipelineEvent::CreatePipelineEvent(Device& device) const
{
    const VkEventCreateInfo even_desc = {
        .sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
    };

    VkEvent event = nullptr;
    VK_CALL(vkCreateEvent(device, &even_desc, nullptr, &event), "Failed to create Vulkan Pipeline Sync Event");
    return event;
}

bool PipelineEvent::HasBeenSignaled() const
{
    VkResult result = vkGetEventStatus(*this->m_device_ref, this->m_event);
    switch (result) {
        case (VK_EVENT_SET): {
            return true;
            break;
        }
        case (VK_EVENT_RESET): {
            return false;
            break;
        }
        default: {
            LOG_ERROR("Failed to fetch current state of pipeline sync event. Error code = %d", (uint32)result);
#ifndef NDEBUG
            TRIGGER_DEBUG_TRAP();
            abort();
#else
            return false;
#endif
            break;
        }
    }
}

void PipelineEvent::Signal()
{
    VK_CALL(vkSetEvent(*this->m_device_ref, this->m_event), "Failed to signal vulkan pipeline event");
}

void PipelineEvent::Reset()
{
    VK_CALL(vkResetEvent(*this->m_device_ref, this->m_event), "Failed to reset vulkan pipeline event");
}

DeviceSemaphore::DeviceSemaphore(Device& device)
{
    this->m_device_ref = &device;
    this->m_semaphore = this->CreateSemaphore(device);
}

DeviceSemaphore::~DeviceSemaphore()
{
    if (this->m_semaphore != nullptr) {
        assert(this->m_device_ref != nullptr);
        vkDestroySemaphore(*this->m_device_ref, this->m_semaphore, nullptr);
    }
}

DeviceSemaphore::DeviceSemaphore(DeviceSemaphore&& other)
{
    if (this == &other)
        return;

    this->m_semaphore = other.m_semaphore;
    this->m_device_ref = other.m_device_ref;
    other.m_semaphore = nullptr;
    other.m_device_ref = nullptr;
}

DeviceSemaphore& DeviceSemaphore::operator=(DeviceSemaphore&& other)
{
    if (this == &other)
        return *this;

    this->m_semaphore = other.m_semaphore;
    this->m_device_ref = other.m_device_ref;
    other.m_semaphore = nullptr;
    other.m_device_ref = nullptr;

    return *this;
}

VkSemaphore DeviceSemaphore::CreateSemaphore(Device& device) const
{
    const VkSemaphoreCreateInfo semaphore_desc = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
    };

    VkSemaphore semaphore = nullptr;
    VK_CALL(vkCreateSemaphore(device, &semaphore_desc, nullptr, &semaphore),
            "Failed to create a Vulkan Device Semaphore");
    return semaphore;
}

}
}
}
