#include "Window.h"

#include "GLFW/glfw3.h"
#include <cassert>
#include <unordered_map>

#include <Rendering/API/Vulkan/Helpers.h>
#include <Rendering/API/Vulkan/Instance.h>

#include "Log.h"

/* NOTE(Tiago): seems that on macos the even poll function
 * can only be called from the main thread. Unlike on windows,
 * moving windows creation to the dedicated window thread does
 * not allow us to get around this limitation, which means that
 * we will need to explicitly call the update function from the
 * main thread */

namespace SHD {
namespace Renderer {

Window::Window(std::string_view title, int32 width, int32 height) :
    width((uint32)width),
    height((uint32)height)
{
    static std::unordered_map<GLFWwindow*, Window*> g_window_handle_map;
    glfwInit();

    /*NOTE(Tiago): Hint must be set otherwise an OpenGL Context will be created
     * which in turn prevents from creating a vulkan surface with this window*/
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    this->m_window_handle = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
    g_window_handle_map[this->m_window_handle] = this;
    this->m_is_open = true;

    glfwSetWindowCloseCallback(this->m_window_handle, [](GLFWwindow* window) {
        Window* current = g_window_handle_map[window];
        assert(current != nullptr);

        current->m_is_open = false;
        glfwDestroyWindow(window);
    });

    glfwSetWindowSizeCallback(this->m_window_handle, [](GLFWwindow* window, int32 new_width, int32 new_height) {
        Window* current = g_window_handle_map[window];
        assert(current != nullptr);

        glfwGetFramebufferSize(window, &new_width, &new_height);
        current->width = (uint32)new_width;
        current->height = (uint32)new_height;
    });
}

Window::~Window()
{
    if (this->m_window_handle != nullptr && this->m_is_open) {
        glfwDestroyWindow(this->m_window_handle);
    }
}

void Window::Update()
{
    if (this->m_is_open) {
        glfwSwapBuffers(this->m_window_handle);
        glfwPollEvents();
    }
}

void Window::Resize(int32 width, int32 height)
{
    assert(width > 0 && height > 0);
    assert(this->m_window_handle != nullptr);
    assert(this->m_is_open);

    glfwSetWindowSize(this->m_window_handle, width, height);
    this->width = (uint32)width;
    this->height = (uint32)height;
}

void Window::SetTitle(std::string_view title)
{
    assert(title.data() != nullptr);
    assert(this->m_window_handle != nullptr);
    assert(this->m_is_open);

    glfwSetWindowTitle(this->m_window_handle, title.data());
}

bool Window::IsOpen() const
{
    return this->m_is_open;
}

VkSurfaceKHR Window::CreateVulkanSurface()
{
    assert(this->m_window_handle != nullptr);
    assert(this->m_is_open);

    VkSurfaceKHR surface = nullptr;
    VK_CALL(glfwCreateWindowSurface(Vulkan::Instance::GetInstance(), this->m_window_handle, nullptr, &surface),
            "Failed to create Vulkan Surface from GLFW Window");
    return surface;
}

}
}
