function(setup_vulkan_dependency
        target_name #input
)
    find_package(Vulkan REQUIRED)
    if(WIN32)
        set(VK_SDK_PATH "$ENV{VULKAN_SDK}")
        target_include_directories(${target_name} SYSTEM PRIVATE ${VK_SDK_PATH}/Include/)
        target_link_libraries(${target_name} PRIVATE ${VK_SDK_PATH}/Lib/vulkan-1.lib)
    else()
        target_include_directories(${target_name} SYSTEM PRIVATE ${Vulkan_INCLUDE_DIRS})
        target_link_libraries(${target_name} PRIVATE Vulkan::Vulkan)
    endif()
endfunction()

