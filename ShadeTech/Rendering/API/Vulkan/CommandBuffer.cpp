#include "CommandBuffer.h"

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
    vkDestroyCommandPool(*this->m_device_ref, this->m_command_poll, nullptr);
}

VkCommandPool CommandPool::CreateCommandPool(Device& device, uint8 queue_index)
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

}
}
}
