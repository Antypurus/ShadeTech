#pragma once

#include <vulkan/vulkan.h>

#include <array>
#include <vector>
#include <string>

namespace SHD
{
namespace Renderer
{
namespace Vulkan
{

static const char* const layers_to_enable[] = {
    "VK_LAYER_KHRONOS_validation"
};

static const char* const extensions_to_enable[] = {
    "VK_KHR_portability_enumeration"
};

struct Instance
{
public:
    std::vector<std::string> supported_layers;
    std::vector<std::string> supported_extensions;

private:
    VkInstance m_instance = nullptr;

// instance methods
public:
    Instance();

    inline VkInstance& GetInstance() {
        return this->m_instance;
    }

private:
    void CreateInstance();


// static methods
public:
    static std::vector<std::string> GetSupportedExtensions();
    static std::vector<std::string> GetSupportedLayers();
};

}
}
}
