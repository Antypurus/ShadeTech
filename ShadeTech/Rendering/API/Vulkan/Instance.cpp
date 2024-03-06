#include "Instance.h"

#include "Helpers.h"

namespace SHD
{
namespace Renderer
{
namespace Vulkan
{

static const size_t count_of_layers_to_enable = sizeof(layers_to_enable) / sizeof(layers_to_enable[0]);
static const size_t count_of_extensions_to_enable = sizeof(extensions_to_enable) / sizeof(extensions_to_enable[0]);

Instance::Instance()
    : supported_layers(Instance::GetSupportedLayers()), supported_extensions(Instance::GetSupportedExtensions())
{
    this->CreateInstance();
}

void Instance::CreateInstance()
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

    const VkInstanceCreateInfo instance_info{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = count_of_layers_to_enable,
        .ppEnabledLayerNames = (char**)layers_to_enable,
        .enabledExtensionCount = count_of_extensions_to_enable,
        .ppEnabledExtensionNames = (char**)extensions_to_enable,
    };

    VK_CALL(vkCreateInstance(&instance_info, nullptr, &this->m_instance), "Failed to create Vulkan Instance");
}

std::vector<std::string> Instance::GetSupportedExtensions()
{
    unsigned int extension_count = 0;

    VK_CALL(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr), "");

    std::vector<VkExtensionProperties> extensions(extension_count);
    VK_CALL(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data()), "");

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
    VK_CALL(vkEnumerateInstanceLayerProperties(&layer_count, nullptr), "");

    std::vector<VkLayerProperties> layers(layer_count);
    VK_CALL(vkEnumerateInstanceLayerProperties(&layer_count, layers.data()), "");

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
