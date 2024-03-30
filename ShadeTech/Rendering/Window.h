#pragma once

#include <GLFW/glfw3.h>
#include <thread>

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
    std::thread m_window_thread;

public:
    Window(std::string_view title, int32 width, int32 height);
    ~Window();

    void Update();
    bool IsOpen() const;
};

}
}
