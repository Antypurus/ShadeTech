#include <iostream>
#include <GLFW/glfw3.h>

#include "Rendering/API/Vulkan/VulkanAPI.h"

using namespace SHD;
using namespace SHD::Renderer;

int main()
{
    std::cout << "Welcome To the SDH Network Agent\n";

    if(glfwInit() == 0)
    {
        std::cout << "Failed to init GLFW\n";
        return -1;
    }
    std::cout << "GLFW Initialized\n";

    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if(window == nullptr)
    {
        // Window or OpenGL context creation failed
    }

    Vulkan::Instance::ListSupportedExtensions();
    Vulkan::Instance instance;

    while(glfwWindowShouldClose(window) == 0)
    {
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
