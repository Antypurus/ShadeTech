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

    this->PopulateSupportedLayerList();
    this->PopulateSupportedExtensionList();
    this->PopulateQueueFamilyList();
}

bool PhysicalDeviceInfo::IsExtensionSupported(const char* queried_extension) const
{
    for (const std::string& extension : this->supported_extensions) {
        if (extension == queried_extension) {
            return true;
        }
    }
    return false;
}

// TODO(Tiago): really dont like the way that we are reporting that no queue supports the surface here
// should rethink this interface in the future. Maybe with a Result<T>
int32 PhysicalDeviceInfo::IsSurfaceSupported(VkSurfaceKHR surface) const
{
    for (uint32 i = 0; i < this->queue_famillies.size(); ++i) {
        VkBool32 supported = VK_FALSE;
        VK_CALL(vkGetPhysicalDeviceSurfaceSupportKHR(this->device_handle, i, surface, &supported),
                "Failed to determine if surface is supported");
        if (supported == VK_TRUE) {
            return (int32)i;
        }
    }
    return -1;
}

std::vector<VkPresentModeKHR> PhysicalDeviceInfo::GetSurfacePresentationModes(VkSurfaceKHR surface) const
{
    uint32 present_mode_count = 0;
    VK_CALL(vkGetPhysicalDeviceSurfacePresentModesKHR(this->device_handle, surface, &present_mode_count, nullptr),
            "Failed to query number of supported presentation modes for surface");

    std::vector<VkPresentModeKHR> presentation_modes(present_mode_count);
    VK_CALL(vkGetPhysicalDeviceSurfacePresentModesKHR(
                this->device_handle, surface, &present_mode_count, presentation_modes.data()),
            "Failed to query presentation modes for surface");

    return presentation_modes;
}

void PhysicalDeviceInfo::PopulateQueueFamilyList()
{
    uint32 queue_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(this->device_handle, &queue_count, nullptr);

    this->queue_famillies.resize(queue_count);
    vkGetPhysicalDeviceQueueFamilyProperties(this->device_handle, &queue_count, this->queue_famillies.data());
}

void PhysicalDeviceInfo::PopulateSupportedLayerList()
{
    uint32 layer_count = 0;
    VK_CALL(vkEnumerateDeviceLayerProperties(this->device_handle, &layer_count, nullptr),
            "Failed to fetch device extension count");

    this->supported_layers.reserve(layer_count);

    std::vector<VkLayerProperties> layers(layer_count);
    VK_CALL(vkEnumerateDeviceLayerProperties(this->device_handle, &layer_count, layers.data()),
            "Failed to fetch device layers");

    for (size_t i = 0; i < layer_count; ++i) {
        this->supported_layers.emplace_back(layers[i].layerName);
    }
}

void PhysicalDeviceInfo::PopulateSupportedExtensionList()
{
    uint32 extension_count = 0;
    VK_CALL(vkEnumerateDeviceExtensionProperties(this->device_handle, nullptr, &extension_count, nullptr),
            "Failed to fetch base vulkan extension count");

    std::vector<VkExtensionProperties> extensions(extension_count);
    VK_CALL(vkEnumerateDeviceExtensionProperties(this->device_handle, nullptr, &extension_count, extensions.data()),
            "Failed to fetch base vulkan extensions");

    for (size_t i = 0; i < extension_count; ++i) {
        this->supported_extensions.emplace_back(extensions[i].extensionName);
    }

    for (const std::string& layer : this->supported_layers) {
        VK_CALL(vkEnumerateDeviceExtensionProperties(this->device_handle, layer.c_str(), &extension_count, nullptr),
                "Failed to fetch layer extension count");

        extensions.resize(extension_count);
        VK_CALL(vkEnumerateDeviceExtensionProperties(
                    this->device_handle, layer.c_str(), &extension_count, extensions.data()),
                "Failed to fetch layer extensions");

        for (size_t i = 0; i < extension_count; ++i) {
            this->supported_extensions.emplace_back(extensions[i].extensionName);
        }
    }
}

void PhysicalDeviceInfo::LogDeviceInformation() const
{
    LOG_INFO("Device Name:%s", (char*)this->device_properties.deviceName);
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

    const VkPhysicalDeviceLimits& limits = this->device_properties.limits;
    LOG_INFO("Device Max 1D Res:%d", limits.maxImageDimension1D);
    LOG_INFO("Device Max 2D Res:%d", limits.maxImageDimension2D);
    LOG_INFO("Device Max 3D Res:%d", limits.maxImageDimension3D);
    LOG_INFO("Device Max CubeMap Res:%d", limits.maxImageDimensionCube);

    LOG_INFO("Number of Queue Families: %zu", this->queue_famillies.size());
    for (size_t i = 0; i < this->queue_famillies.size(); ++i) {
        const VkQueueFamilyProperties& queue = this->queue_famillies[i];
        LOG_INFO("Queue #%zu\n"
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

    LOG_INFO("Number of memory heaps:%d", this->memory_properties.memoryHeapCount);
    for (size_t i = 0; i < this->memory_properties.memoryHeapCount; ++i) {
        const VkMemoryHeap& heap = this->memory_properties.memoryHeaps[i];

        std::string heap_features = "{";
        if (HAS_BITFLAG(heap.flags, VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)) {
            heap_features += "Device Heap";
        } else {
            heap_features += "Host Heap";
        }
        if (HAS_BITFLAG(heap.flags, VK_MEMORY_HEAP_MULTI_INSTANCE_BIT)) {
            heap_features += " | Multi-Device Allocation Replication";
        }
        heap_features += "}";

        LOG_INFO("Heap #%zu\n"
                 "\t Size: %llu MiB\n"
                 "\t Heap Features: %s\n",
                 i,
                 (heap.size / MiB),
                 heap_features.c_str());
    }

    LOG_INFO("Number of types of heap memory:%d", this->memory_properties.memoryTypeCount);
    for (size_t i = 0; i < this->memory_properties.memoryTypeCount; ++i) {
        const VkMemoryType& memory_type = this->memory_properties.memoryTypes[i];

        std::string features = "{";
        if (HAS_BITFLAG(memory_type.propertyFlags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)) {
            features += "Device Memory";
        } else {
            features += "Host Memory";
        }
        if (HAS_BITFLAG(memory_type.propertyFlags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)) {
            features += " | Host Visible";
        }
        if (HAS_BITFLAG(memory_type.propertyFlags, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
            features += " | Automatic Device <-> Host cache flushing";
        }
        if (HAS_BITFLAG(memory_type.propertyFlags, VK_MEMORY_PROPERTY_HOST_CACHED_BIT)) {
            features += " | Host Cached";
        }
        if (HAS_BITFLAG(memory_type.propertyFlags, VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT)) {
            features += " | Device Only Visible & Possibly Lazilly ALlocated";
        }
        if (HAS_BITFLAG(memory_type.propertyFlags, VK_MEMORY_PROPERTY_PROTECTED_BIT)) {
            features += " | Device Only Visible & Allows Protected Queue Ops";
        }
        features += "}";

        LOG_INFO("Memory Type #%zu\n"
                 "\tAssociated Heap Index: %d\n"
                 "\tMemory Type Features: %s\n",
                 i,
                 memory_type.heapIndex,
                 features.c_str());
    }

    for (auto& extension : this->supported_extensions) {
        LOG_INFO("%s", extension.c_str());
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

    VkInstanceCreateFlags flags = 0;
    if (this->IsExtensionSupported(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME)) {
        extensions_to_enable.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    }

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
        .flags = flags,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = count_of_layers_to_enable,
        .ppEnabledLayerNames = (char**)layers_to_enable,
        .enabledExtensionCount = (uint32)extensions_to_enable.size(),
        .ppEnabledExtensionNames = (char**)extensions_to_enable.data(),
    };

    VK_CALL(vkCreateInstance(&instance_info, nullptr, &this->m_instance), "Failed to create Vulkan Instance");
}

bool Instance::IsLayerSupported(const char* queried_layer) const
{
    for (const std::string& layer : this->supported_layers) {
        if (layer == queried_layer) {
            return true;
        }
    }

    return false;
}

bool Instance::IsExtensionSupported(const char* queried_extension) const
{
    for (const std::string& extension : this->supported_extensions) {
        if (extension == queried_extension) {
            return true;
        }
    }

    return false;
}

void Instance::PopulateDeviceList()
{
    uint32 device_count = 0;
    VK_CALL(vkEnumeratePhysicalDevices(this->m_instance, &device_count, nullptr),
            "Failed to fetch physical device count");

    // NOTE(Tiago): used to create an array so that we dont need to allocate
    // such a small vector
    constexpr size_t max_physical_device_count = 16;
    VkPhysicalDevice l_devices[max_physical_device_count];

    VK_CALL(vkEnumeratePhysicalDevices(this->m_instance, &device_count, (VkPhysicalDevice*)l_devices),
            "Failed to fetch handles of installed physical devices");

    this->devices.reserve(device_count);
    for (uint32 i = 0; i < device_count; ++i) {
        this->devices.emplace_back(l_devices[i]);
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

    const std::vector<std::string> layers = Instance::GetSupportedLayers();
    for (const std::string& layer : layers) {
        VK_CALL(vkEnumerateInstanceExtensionProperties(layer.c_str(), &extension_count, nullptr),
                "Failed to fetch layer extension count");

        extensions.resize(extension_count);
        VK_CALL(vkEnumerateInstanceExtensionProperties(layer.c_str(), &extension_count, extensions.data()),
                "Failed to fetch extension properties");

        for (size_t i = 0; i < extension_count; ++i) {
            supported_extensions.emplace_back(extensions[i].extensionName);
        }
    }

    return supported_extensions;
}

std::vector<std::string> Instance::GetSupportedLayers()
{
    unsigned int layer_count = 0;
    VK_CALL(vkEnumerateInstanceLayerProperties(&layer_count, nullptr),
            "Failed to get number of supported instance layers");

    std::vector<VkLayerProperties> layers(layer_count);
    VK_CALL(vkEnumerateInstanceLayerProperties(&layer_count, layers.data()), "Failed to get supported instance layers");

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
