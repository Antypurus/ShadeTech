#pragma once

#include <vulkan/vulkan.h>

#include <string>
#include <vector>

namespace SHD {
namespace Renderer {
namespace Vulkan {

static const char* const layers_to_enable[] = { "VK_LAYER_KHRONOS_validation" };

static const char* const extensions_to_enable[] = { "VK_KHR_portability_enumeration" };

struct PhysicalDeviceInfo
{
public:
    VkPhysicalDevice device_handle = nullptr;
    VkPhysicalDeviceProperties device_properties = {};
    VkPhysicalDeviceFeatures device_features = {};
    VkPhysicalDeviceMemoryProperties memory_properties = {};
    std::vector<VkQueueFamilyProperties> queue_famillies;
    std::vector<std::string> supported_layers;
    std::vector<std::string> supported_extensions;

public:
    PhysicalDeviceInfo(VkPhysicalDevice device_handle);
    VkDevice CreateDevice();

    void LogDeviceInformation() const;

private:
    void PopulateQueueFamilyList();
    void PopulateSupportedLayerList();
    void PopulateSupportedExtensionList();
};

struct Instance
{
public:
    std::vector<PhysicalDeviceInfo> devices;
    std::vector<std::string> supported_layers;
    std::vector<std::string> supported_extensions;

private:
    VkInstance m_instance = nullptr;

public:
    ~Instance();

    Instance(const Instance& other) = delete;
    Instance& operator=(const Instance& other) = delete;

    Instance(Instance&& other) = delete;
    Instance& operator=(Instance&& other) = delete;

    operator VkInstance() { return this->m_instance; }

private:
    Instance();
    void CreateInstance();
    void PopulateDeviceList();

public:
    static inline Instance& GetInstance()
    {
        static Instance s_instance;
        return s_instance;
    }

    static std::vector<std::string> GetSupportedExtensions();
    static std::vector<std::string> GetSupportedLayers();
};

}
}
}
