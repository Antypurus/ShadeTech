#include <iostream>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

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

    /* ========== VULKAN STUFF ================ */
    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    std::cout << "Number of supported extensions:" << extension_count << '\n';


    const VkApplicationInfo app_info {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = "Vykur App",
        .applicationVersion = 1,
        .pEngineName = "Vykur",
        .engineVersion = 1,
        .apiVersion = VK_API_VERSION_1_2,
    };

    std::vector<const char*> extensions;
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    const VkInstanceCreateInfo instance_info
    {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0UL | VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = (uint32_t)extensions.size(),
        .ppEnabledExtensionNames = extensions.data(),
    };

    VkInstance instance = nullptr;
    const VkResult result = vkCreateInstance(&instance_info, nullptr, &instance);
    if(result == VK_SUCCESS)
    {
        std::cout << "Vulkan Instance Created\n";
    } else {
        std::cout << "Failed to create Vulkan Instance\n";
    }
    /* ============ VULKAN STUFF END =================== */


    while(glfwWindowShouldClose(window) == 0)
    {
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
