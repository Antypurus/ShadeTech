#pragma once

#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
#include <string_view>

#include <Types.h>

namespace SHD {
namespace Renderer {

class Window
{
public:
    uint32 width = 0;
    uint32 height = 0;

private:
    GLFWwindow* m_window_handle = nullptr;
    bool m_is_open = false;

public:
    Window(std::string_view title, int32 width, int32 height);
    ~Window();

    void Update();
    void Resize(int32 width, int32 height);
    void SetTitle(std::string_view title);
    bool IsOpen() const;

    VkSurfaceKHR CreateVulkanSurface();
};

}
}
