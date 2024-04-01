#include "Device.h"

#include <cassert>
#include <vulkan/vulkan_core.h>

#include "Helpers.h"
#include "Instance.h"
#include "Types.h"

namespace SHD {
namespace Renderer {
namespace Vulkan {

/* TODOS:
 *  - [ ] Queue Creation Is Pretty much still hard coded to a single queue
 *        Need to come up with a system to determine how many queues to create
 *        and maximize paralelism and still be correct
 */

Device::Device() :
    m_device(this->CreateVKDevice(0))
{
    PhysicalDeviceInfo& physical_device = Instance::GetInstance().devices[0];
    this->m_device_info_ref = &physical_device;
    this->m_command_queue = this->CreateCommandQueue(0, 0);
}

Device::Device(uint32 device_index) :
    m_device(this->CreateVKDevice(device_index))
{
    assert((device_index + 1) <= Instance::GetInstance().devices.size());
    PhysicalDeviceInfo& physical_device = Instance::GetInstance().devices[device_index];
    this->m_device_info_ref = &physical_device;
    this->m_command_queue = this->CreateCommandQueue(0, 0);
}

Device::~Device()
{
    if (this->m_device != nullptr) {
        vkDestroyDevice(this->m_device, nullptr);
    }
}

VkDevice Device::CreateVKDevice(uint32 device_index) const
{
    assert(device_index <= Instance::GetInstance().devices.size());
    const PhysicalDeviceInfo& physical_device = Instance::GetInstance().devices[device_index];

    const float queue_priorities[] = { 0.0 };
    const VkDeviceQueueCreateInfo queue_create_info{
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueFamilyIndex = 0,
        .queueCount = 1,
        .pQueuePriorities = (float*)queue_priorities,
    };

    std::vector<const char*> device_extensions{
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };
    // the portability extension can only be enabled for non-conformant vulkan implementation
    // conformant implementations will not report it. The weird thing is that some conformant
    // implementations will report it at the instance level but then the required device
    // extension will not be reported as being supported, so we need to account of that.
    if (Instance::GetInstance().IsExtensionSupported(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME) &&
        physical_device.IsExtensionSupported("VK_KHR_portability_subset")) {
        device_extensions.push_back("VK_KHR_portability_subset");
    }

    const VkDeviceCreateInfo device_create_info{
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queue_create_info,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = (uint32)device_extensions.size(),
        .ppEnabledExtensionNames = device_extensions.data(),
        .pEnabledFeatures = &physical_device.device_features,
    };

    VkDevice device = nullptr;
    VK_CALL(vkCreateDevice(physical_device.device_handle, &device_create_info, nullptr, &device),
            "Failed to create Vulkan logical device");

    return device;
}

VkQueue Device::CreateCommandQueue(uint32 family_index, uint32 queue_index) const
{
    VkQueue command_queue = nullptr;
    vkGetDeviceQueue(this->m_device, family_index, queue_index, &command_queue);
    return command_queue;
}

}
}
}
