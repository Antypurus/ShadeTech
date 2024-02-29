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
private:
    VkInstance m_instance;

public:
    Instance(const std::vector<std::string>& layers_to_enable = {},
             const std::vector<std::string>& extensions_to_enable = {});

    static std::vector<std::string> GetSupportedExtensions();
    static std::vector<std::string> GetSupportedLayers();
};

}
}
}
