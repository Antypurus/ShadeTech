#include "Window.h"
#include "GLFW/glfw3.h"

#include <unordered_map>

/* NOTE(Tiago): seems that on macos the even poll function
 * can only be called from the main thread. Unlike on windows,
 * moving windows creation to the dedicated window thread does
 * not allow us to get around this limitation, which means that
 * we will need to explicitly call the update function from the
 * main thread */

namespace SHD {
namespace Renderer {

static std::unordered_map<GLFWwindow*, Window*> g_window_handle_map;

Window::Window(std::string_view title, int32 width, int32 height) :
    width((uint32)width),
    height((uint32)height)
{
    glfwInit();

    this->m_window_handle = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
    g_window_handle_map[this->m_window_handle] = this;
    this->m_is_open = true;

    glfwSetWindowCloseCallback(this->m_window_handle, [](GLFWwindow* window) {
        Window* current = g_window_handle_map[window];
        current->m_is_open = false;
        glfwDestroyWindow(window);
    });
}

void Window::Update()
{
    if (this->m_is_open) {
        glfwSwapBuffers(this->m_window_handle);
        glfwPollEvents();
    }
}

bool Window::IsOpen() const
{
    return this->m_is_open;
}

Window::~Window()
{
    if (this->m_window_handle != nullptr && this->m_is_open) {
        glfwDestroyWindow(this->m_window_handle);
    }
}

}
}
