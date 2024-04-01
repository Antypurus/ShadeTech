#pragma once

#include <vulkan/vulkan_core.h>
#include <string>
#include <vector>

#include "Platform.h"
#include <Types.h>

namespace SHD {
namespace Renderer {
namespace Vulkan {

static const char* const layers_to_enable[] = { "VK_LAYER_KHRONOS_validation" };

static std::vector<const char*> extensions_to_enable = {
    "VK_KHR_surface",
#if PLATFORM_MACOS
    "VK_EXT_metal_surface",
#endif
#if PLATFORM_WINDOWS
    "VK_KHR_win32_surface",
#endif
};

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

    bool IsExtensionSupported(const char* queried_extension) const;
    int32 IsSurfaceSupported(VkSurfaceKHR surface) const;
    std::vector<VkPresentModeKHR> GetSurfacePresentationModes(VkSurfaceKHR surface) const;
    VkSurfaceCapabilitiesKHR GetSurfaceCapabilities(VkSurfaceKHR surface) const;
    std::vector<VkSurfaceFormatKHR> GetSupportedSurfaceFormats(VkSurfaceKHR surface) const;

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

    bool IsLayerSupported(const char* queried_layer) const;
    bool IsExtensionSupported(const char* queried_extension) const;
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
