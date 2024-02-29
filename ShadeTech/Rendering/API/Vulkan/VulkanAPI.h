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

struct Instance
{
public:
    VkInstance m_instance = nullptr;

// instance methods
public:
    Instance() = default;
    Instance(const std::vector<std::string>& layers_to_enable = {},
             const std::vector<std::string>& extensions_to_enable = {});

    

// static methods
public:
    static std::vector<std::string> GetSupportedExtensions();
    static std::vector<std::string> GetSupportedLayers();
};

}
}
}
