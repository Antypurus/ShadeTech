#include "Window.h"
#include "GLFW/glfw3.h"

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
    glfwInit();
    this->m_window_handle = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
}

void Window::Update()
{
    glfwSwapBuffers(this->m_window_handle);
    glfwPollEvents();
}

bool Window::IsOpen() const
{
    return !glfwWindowShouldClose(this->m_window_handle);
}

Window::~Window()
{
    if (this->m_window_handle != nullptr) {
        glfwDestroyWindow(this->m_window_handle);
    }
}

}
}
