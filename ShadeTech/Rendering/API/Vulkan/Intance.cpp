#include "Instance.h"

#include "Helpers.h"

#include <iostream>
#include <vulkan/vulkan_core.h>

namespace SHD
{
namespace Renderer
{
namespace Vulkan
{

Instance::Instance(const std::vector<std::string>& layers_to_enable,
                   const std::vector<std::string>& extensions_to_enable)
{
    const VkApplicationInfo app_info{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = "Vykur App",
        .applicationVersion = 1,
        .pEngineName = "Vykur",
        .engineVersion = 1,
        .apiVersion = VK_API_VERSION_1_2,
    };

    std::vector<const char*> layers(layers_to_enable.size());
    for(size_t i = 0; i < layers_to_enable.size(); ++i)
    {
        layers[i] = layers_to_enable[i].data();
    }

    std::vector<const char*> extensions(extensions_to_enable.size());
    for(size_t i = 0; i < extensions_to_enable.size(); ++i)
    {
        extensions[i] = extensions_to_enable[i].data();
    }

    const VkInstanceCreateInfo instance_info{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = (uint32_t)layers.size(),
        .ppEnabledLayerNames = layers.data(),
        .enabledExtensionCount = (uint32_t)extensions.size(),
        .ppEnabledExtensionNames = extensions.data(),
    };

    VK_CALL(vkCreateInstance(&instance_info, nullptr, &this->m_instance));
}

std::vector<std::string> Instance::GetSupportedExtensions()
{
    unsigned int extension_count = 0;

    VK_CALL(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr))

    std::vector<VkExtensionProperties> extensions(extension_count);
    VK_CALL(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data()));

    std::vector<std::string> supported_extensions;
    supported_extensions.reserve(extension_count);

    for(VkExtensionProperties& extension : extensions)
    {
        supported_extensions.emplace_back(extension.extensionName);
    }

    return supported_extensions;
}

std::vector<std::string> Instance::GetSupportedLayers()
{
    unsigned int layer_count = 0;

    VkResult enum_result = vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    if(enum_result != VK_SUCCESS)
    {
        std::cout << "failed to fetch layer count\n";
        return {};
    }

    std::vector<VkLayerProperties> layers(layer_count);
    enum_result = vkEnumerateInstanceLayerProperties(&layer_count, layers.data());
    if(enum_result != VK_SUCCESS)
    {
        std::cout << "failed to fetch supported layers\n";
        return {};
    }

    std::vector<std::string> supported_layers;
    supported_layers.reserve(layer_count);

    for(VkLayerProperties& layer : layers)
    {
        supported_layers.emplace_back(layer.layerName);
    }

    return supported_layers;
}

}
}
}
