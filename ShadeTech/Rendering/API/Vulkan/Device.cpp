#include "Device.h"

#include <cassert>
#include <vulkan/vulkan_core.h>

#include "Helpers.h"
#include "Instance.h"

namespace SHD {
namespace Renderer {
namespace Vulkan {

Device::Device() :
    m_device(this->CreateVKDevice(0))
{
}

Device::Device(uint32 device_index) :
    m_device(this->CreateVKDevice(device_index))
{
    assert(device_index <= Instance::GetInstance().devices.size());
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

    std::vector<const char*> device_extensions;
    // the portability extension can only be enabled for non-conformant vulkan implementation
    // conformant implementations will not report it.
    if (Instance::GetInstance().IsExtensionSupported(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME)) {
        device_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
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

}
}
}
