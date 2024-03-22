#include "Instance.h"

#include "Helpers.h"
#include <Types.h>
#include <vulkan/vulkan_core.h>

namespace SHD {
namespace Renderer {
namespace Vulkan {

PhysicalDeviceInfo::PhysicalDeviceInfo(VkPhysicalDevice device_handle) :
    device_handle(device_handle)
{
    vkGetPhysicalDeviceFeatures(this->device_handle, &this->device_features);
    vkGetPhysicalDeviceProperties(this->device_handle, &this->device_properties);
    vkGetPhysicalDeviceMemoryProperties(this->device_handle, &this->memory_properties);

    this->PopulateQueueFamilyList();
}

VkDevice PhysicalDeviceInfo::CreateDevice()
{
    const VkDeviceCreateInfo device_create_info{
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueCreateInfoCount = 0,
        .pQueueCreateInfos = nullptr,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = 0,
        .ppEnabledExtensionNames = nullptr,
        .pEnabledFeatures = nullptr,
    };

    VkDevice result_device = nullptr;
    VK_CALL(vkCreateDevice(this->device_handle, &device_create_info, nullptr, &result_device),
            "Failed to create Vulkan Logical Device");

    return result_device;
}

void PhysicalDeviceInfo::PopulateQueueFamilyList()
{
    uint32 queue_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(this->device_handle, &queue_count, nullptr);

    this->queue_famillies.resize(queue_count);
    vkGetPhysicalDeviceQueueFamilyProperties(this->device_handle, &queue_count, this->queue_famillies.data());
}

void PhysicalDeviceInfo::LogDeviceInformation() const
{
    LOG_INFO("Device Name:%s", this->device_properties.deviceName);
    switch (this->device_properties.deviceType) {
        case (VK_PHYSICAL_DEVICE_TYPE_CPU): {
            LOG_INFO("Device Type: CPU");
            break;
        }
        case (VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU): {
            LOG_INFO("Device Type: Integrated GPU");
            break;
        }
        case (VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU): {
            LOG_INFO("Device Type: Discrete GPU");
            break;
        }
        case (VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU): {
            LOG_INFO("Device Type: Virtual GPU");
            break;
        }
        default: {
            LOG_INFO("Device Type: Unknown");
            break;
        }
    }

    LOG_INFO("Number of Queue Families: %lu", this->queue_famillies.size());
    for (size_t i = 0; i < this->queue_famillies.size(); ++i) {
        const VkQueueFamilyProperties& queue = this->queue_famillies[i];
        LOG_INFO("Queue #%lu\n"
                 "\t Queue Count:%d\n"
                 "\t Graphics Queue:%d\n"
                 "\t Compute Queue:%d\n"
                 "\t Transfer Queue:%d\n"
                 "\t Sparse Binding Queue:%d\n"
                 "\t Video Decode Queue%d\n"
                 "\t Optical Flow Queue:%d",
                 i,
                 queue.queueCount,
                 HAS_BITFLAG(queue.queueFlags, VK_QUEUE_GRAPHICS_BIT),
                 HAS_BITFLAG(queue.queueFlags, VK_QUEUE_COMPUTE_BIT),
                 HAS_BITFLAG(queue.queueFlags, VK_QUEUE_TRANSFER_BIT),
                 HAS_BITFLAG(queue.queueFlags, VK_QUEUE_SPARSE_BINDING_BIT),
                 HAS_BITFLAG(queue.queueFlags, VK_QUEUE_VIDEO_DECODE_BIT_KHR),
                 HAS_BITFLAG(queue.queueFlags, VK_QUEUE_OPTICAL_FLOW_BIT_NV));
    }
}

Instance::Instance() :
    supported_layers(Instance::GetSupportedLayers()),
    supported_extensions(Instance::GetSupportedExtensions())
{
    this->CreateInstance();
    this->PopulateDeviceList();
}

Instance::~Instance()
{
    vkDestroyInstance(this->m_instance, nullptr);
}

void Instance::CreateInstance()
{
    static const size_t count_of_layers_to_enable = sizeof(layers_to_enable) / sizeof(layers_to_enable[0]);
    static const size_t count_of_extensions_to_enable = sizeof(extensions_to_enable) / sizeof(extensions_to_enable[0]);

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

void Instance::PopulateDeviceList()
{
    uint32 device_count = 0;
    VK_CALL(vkEnumeratePhysicalDevices(this->m_instance, &device_count, nullptr),
            "Failed to fetch physical device count");

    // NOTE(Tiago): used to create an array so that we dont need to allocate
    // such a small vector
    constexpr size_t max_physical_device_count = 16;
    VkPhysicalDevice devices[max_physical_device_count];

    VK_CALL(vkEnumeratePhysicalDevices(this->m_instance, &device_count, (VkPhysicalDevice*)devices),
            "Failed to fetch handles of installed physical devices");

    this->devices.reserve(device_count);
    for (uint32 i = 0; i < device_count; ++i) {
        this->devices.emplace_back(devices[i]);
    }
}

std::vector<std::string> Instance::GetSupportedExtensions()
{
    unsigned int extension_count = 0;

    VK_CALL(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr),
            "Failed to get number of supported extensions");

    std::vector<VkExtensionProperties> extensions(extension_count);
    VK_CALL(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data()),
            "Failed to get supported extensions");

    std::vector<std::string> supported_extensions;
    supported_extensions.reserve(extension_count);

    for (VkExtensionProperties& extension : extensions) {
        supported_extensions.emplace_back(extension.extensionName);
    }

    return supported_extensions;
}

std::vector<std::string> Instance::GetSupportedLayers()
{
    unsigned int layer_count = 0;
    VK_CALL(vkEnumerateInstanceLayerProperties(&layer_count, nullptr), "Failed to get number of supported layers");

    std::vector<VkLayerProperties> layers(layer_count);
    VK_CALL(vkEnumerateInstanceLayerProperties(&layer_count, layers.data()), "Failed to get supported layers");

    std::vector<std::string> supported_layers;
    supported_layers.reserve(layer_count);

    for (VkLayerProperties& layer : layers) {
        supported_layers.emplace_back(layer.layerName);
    }

    return supported_layers;
}

} // namespace Vulkan
} // namespace Renderer
} // namespace SHD
