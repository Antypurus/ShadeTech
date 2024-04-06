#include "CommandBuffer.h"

#include <cassert>
#include <vulkan/vulkan_core.h>

#include "Helpers.h"

namespace SHD {
namespace Renderer {
namespace Vulkan {

CommandPool::CommandPool(Device& device, uint8 queue_index) :
    m_command_poll(this->CreateCommandPool(device, queue_index)),
    m_device_ref(&device)
{
}

CommandPool::~CommandPool()
{
    if (this->m_command_poll != nullptr) {
        vkDestroyCommandPool(*this->m_device_ref, this->m_command_poll, nullptr);
    }
}

VkCommandPool CommandPool::CreateCommandPool(Device& device, uint8 queue_index) const
{
    VkCommandPoolCreateInfo command_pool_descripion = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = queue_index,
    };

    VkCommandPool command_poll = nullptr;
    VK_CALL(vkCreateCommandPool(device, &command_pool_descripion, nullptr, &command_poll),
            "Failed to create Vulkan Command Pool");
    return command_poll;
}

CommandPool::CommandPool(CommandPool&& other)
{
    if (this == &other)
        return;

    this->m_command_poll = other.m_command_poll;
    this->m_device_ref = other.m_device_ref;
    other.m_command_poll = nullptr;
    other.m_device_ref = nullptr;
}

CommandPool& CommandPool::operator=(CommandPool&& other)
{
    if (this == &other)
        return *this;

    this->m_command_poll = other.m_command_poll;
    this->m_device_ref = other.m_device_ref;
    other.m_command_poll = nullptr;
    other.m_device_ref = nullptr;

    return *this;
}

CommandBuffer::CommandBuffer(Device& device, CommandPool& command_pool) :
    m_command_buffer(this->CreateCommandBuffer(device, command_pool)),
    m_device_ref(&device),
    m_command_pool_ref(&command_pool)
{
    assert(device != nullptr);
    assert(command_pool != nullptr);
}

CommandBuffer::~CommandBuffer()
{
    if (this->m_command_buffer != nullptr) {
        vkFreeCommandBuffers(*m_device_ref, *m_command_pool_ref, 1, &this->m_command_buffer);
    }
}

void CommandBuffer::BeginRecording()
{
    const VkCommandBufferBeginInfo recording_desc = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pInheritanceInfo = nullptr,
    };
    VK_CALL(vkBeginCommandBuffer(this->m_command_buffer, &recording_desc), "Failed to start recording command buffer");
}

void CommandBuffer::StopRecording()
{
    VK_CALL(vkEndCommandBuffer(this->m_command_buffer), "Failed to stop recording command buffer");
}

void CommandBuffer::Reset()
{
    VK_CALL(vkResetCommandBuffer(this->m_command_buffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT),
            "Failed to reset command buffer");
}

VkCommandBuffer CommandBuffer::CreateCommandBuffer(Device& device, CommandPool& command_pool) const
{
    const VkCommandBufferAllocateInfo command_buffer_desc = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = command_pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    VkCommandBuffer command_buffer = nullptr;
    VK_CALL(vkAllocateCommandBuffers(device, &command_buffer_desc, &command_buffer),
            "Failed to allocate command buffer");
    return command_buffer;
}

CommandBuffer::CommandBuffer(CommandBuffer&& other)
{
    if (this == &other)
        return;

    this->m_command_buffer = other.m_command_buffer;
    this->m_device_ref = other.m_device_ref;
    this->m_command_pool_ref = other.m_command_pool_ref;
    other.m_command_buffer = nullptr;
    other.m_device_ref = nullptr;
    other.m_command_pool_ref = nullptr;
}

CommandBuffer& CommandBuffer::operator=(CommandBuffer&& other)
{
    if (this == &other)
        return *this;

    this->m_command_buffer = other.m_command_buffer;
    this->m_device_ref = other.m_device_ref;
    this->m_command_pool_ref = other.m_command_pool_ref;
    other.m_command_buffer = nullptr;
    other.m_device_ref = nullptr;
    other.m_command_pool_ref = nullptr;

    return *this;
}

}
}
}
