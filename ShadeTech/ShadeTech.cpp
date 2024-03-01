#include <iostream>

#include "Rendering/API/Vulkan/VulkanAPI.h"
#include "Log.h"

using namespace SHD;
using namespace SHD::Renderer;

int main()
{
    std::cout << "Welcome To the SDH Network Agent\n";

    Vulkan::Instance instance;

    uint32_t device_count = 0;
    VkResult result = vkEnumeratePhysicalDevices(instance.m_instance, &device_count, nullptr);
    if(result != VK_SUCCESS)
    {
        LOG_ERROR("failed to fetch device count");
        return 1;
    }

    std::vector<VkPhysicalDevice> physical_devices(device_count);
    result = vkEnumeratePhysicalDevices(instance.m_instance, &device_count, physical_devices.data());
    if(result != VK_SUCCESS)
    {
        LOG_ERROR("Failed to fetch physical devices");
        return 1;
    }

    LOG_SUCCESS("device count: %zu\n", physical_devices.size());

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(physical_devices[0], &properties);

    LOG_WARN("%s", (char*)properties.deviceName);

    return 0;
}
