#pragma once

#include <vulkan/vulkan.h>

#include <vector>
#include <string>

namespace SHD
{
namespace Renderer
{
namespace Vulkan
{

static const char* const layers_to_enable[] = {"VK_LAYER_KHRONOS_validation"};

static const char* const extensions_to_enable[] = {"VK_KHR_portability_enumeration"};

struct PhysicalDeviceInfo
{
public:
    VkPhysicalDevice device_handle = nullptr;
    VkPhysicalDeviceProperties device_properties;
    VkPhysicalDeviceFeatures device_features;

public:
    PhysicalDeviceInfo(VkPhysicalDevice device_handle);
};

struct Instance
{
public:
    std::vector<PhysicalDeviceInfo> devices;
    std::vector<std::string> supported_layers;
    std::vector<std::string> supported_extensions;

private:
    VkInstance m_instance = nullptr;

    // instance methods
public:
    Instance();
    ~Instance();

    inline VkInstance& GetInstance()
    {
        return this->m_instance;
    }

private:
    void CreateInstance();
    void PopulateDeviceList();

    // static methods
public:
    static std::vector<std::string> GetSupportedExtensions();
    static std::vector<std::string> GetSupportedLayers();
};

}
}
}
