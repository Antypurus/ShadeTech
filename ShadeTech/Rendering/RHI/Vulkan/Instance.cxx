module;

#include <vulkan/vulkan.h>

export module vulkan;

namespace SHD::Rendering::RHI::Vulkan {

export class Instance
{
private:
    // VkInstance m_instance{};

public:
    Instance()
    {
        // VkInstanceCreateInfo instance_desc = {
        //     .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        //     .pNext = nullptr,
        //     .flags = 0,
        //     .pApplicationInfo = nullptr,
        //     .enabledLayerCount = 0,
        //     .ppEnabledLayerNames = nullptr,
        //     .enabledExtensionCount = 0,
        //     .ppEnabledExtensionNames = nullptr,
        // };
    }
};

}
