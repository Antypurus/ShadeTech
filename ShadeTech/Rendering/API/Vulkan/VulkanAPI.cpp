#include "VulkanAPI.h"

#include <stdlib.h>
#include <iostream>
#include <vulkan/vulkan_core.h>

namespace SHD
{
namespace Renderer
{
namespace Vulkan
{

Instance::Instance()
{
    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    std::cout << "Number of supported extensions:" << extension_count << '\n';

    const VkApplicationInfo app_info{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = "Vykur App",
        .applicationVersion = 1,
        .pEngineName = "Vykur",
        .engineVersion = 1,
        .apiVersion = VK_API_VERSION_1_2,
    };

    std::vector<const char*> extensions;
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    const VkInstanceCreateInfo instance_info{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0UL | VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = (uint32_t)extensions.size(),
        .ppEnabledExtensionNames = extensions.data(),
    };

    const VkResult result = vkCreateInstance(&instance_info, nullptr, &this->m_instance);
    if(result == VK_SUCCESS)
    {
        std::cout << "Vulkan Instance Created\n";
    } else
    {
        std::cout << "Failed to create Vulkan Instance\n";
    }
}

std::vector<std::string> Instance::ListSupportedExtensions()
{
    unsigned int extension_count = 0;

    VkResult enum_result = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    if(enum_result != VK_SUCCESS)
    {
        std::cout << "failed to fetch extension count\n";
        return {};
    }

    std::vector<VkExtensionProperties> extensions(extension_count);
    enum_result = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
    if(enum_result != VK_SUCCESS)
    {
        std::cout << "failed to fetch extensions";
        return {};
    }

    for(VkExtensionProperties& extension : extensions)
    {
        std::cout << extension.extensionName << std::endl;
    }

    return {};
}

}
}
}
