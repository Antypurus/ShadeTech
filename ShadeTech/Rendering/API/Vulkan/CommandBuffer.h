#pragma once

#include <vulkan/vulkan_core.h>

#include "Device.h"
#include "Synchronization.h"
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

    operator VkCommandPool&() { return this->m_command_poll; }

    void Reset();

private:
    VkCommandPool CreateCommandPool(Device& device, uint8 queue_index) const;

public:
    // deleted constructors & operators
    CommandPool(const CommandPool& other) = delete;
    CommandPool& operator=(const CommandPool& other) = delete;
};

class CommandBuffer
{
private:
    VkCommandBuffer m_command_buffer = nullptr;
    Device* m_device_ref = nullptr;
    CommandPool* m_command_pool_ref = nullptr;

public:
    CommandBuffer() = default;
    CommandBuffer(Device& device, CommandPool& command_pool);
    ~CommandBuffer();

    CommandBuffer(CommandBuffer&& other);
    CommandBuffer& operator=(CommandBuffer&& other);

    operator VkCommandBuffer&() { return this->m_command_buffer; }

    void BeginRecording();
    void StopRecording();
    void Reset();

    void SignalPipelineEvent(
        PipelineEvent& event /* TODO(Tiago):need to set a system for specifying pipeline stages */);
    void ResetPipelineEvent(PipelineEvent& event /* TODO(Tiago):need to set a system for specifying pipeline stages */);

private:
    VkCommandBuffer CreateCommandBuffer(Device& device, CommandPool& command_pool) const;

public:
    // deleted constructors & operators
    CommandBuffer(const CommandBuffer& other) = delete;
    CommandBuffer& operator=(const CommandBuffer& other) = delete;
};

}
}
}
