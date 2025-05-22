if(APPLE)
    find_library(COCOA_FRAMEWORK Cocoa)
    find_library(METAL_FRAMEWORK Metal)
    find_library(QUARTZCORE_FRAMEWORK QuartzCore)
endif()

file(GLOB_RECURSE SHD_GLOBAL_MODULE_SOURCES CONFIGURE_DEPENDS
    **.cpp
    **.h)
file(GLOB_RECURSE SHD_OBJC_SOURCES CONFIGURE_DEPENDS **.mm)
extract_platform_dependent_sources(
    "${SHD_GLOBAL_MODULE_SOURCES}"
    SHD_GENERAL_SOURCES
    SHD_WINDOWS_SOURCES
    SHD_LINUX_SOURCES
    SHD_MACOS_SOURCES
    SHD_POSIX_SOURCES)

set(SDH_MODULE_SOURCES)
list(APPEND SHD_MODULE_SOURCES ${SHD_GENERAL_SOURCES})
if(WIN32)
    list(APPEND SHD_MODULE_SOURCES ${SHD_WINDOWS_SOURCES})
elseif(APPLE)
    list(APPEND SHD_MODULE_SOURCES ${SHD_MACOS_SOURCES})
    list(APPEND SHD_MODULE_SOURCES ${SHD_POSIX_SOURCES})
    list(APPEND SHD_MODULE_SOURCES ${SHD_OBJC_SOURCES})
elseif(UNIX)
    list(APPEND SHD_MODULE_SOURCES ${SHD_LINUX_SOURCES})
    list(APPEND SHD_MODULE_SOURCES ${SHD_POSIX_SOURCES})
endif()

add_library(${CMAKE_PROJECT_NAME} ${SHD_MODULE_SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ../)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC glfw ${GLFW_LIBRARIES} cpptrace::cpptrace)
configure_compiler_warnings(${CMAKE_PROJECT_NAME})
if(APPLE)
    set_source_files_properties(${SHD_OBJC_SOURCES} PROPERTIES COMPILE_FLAGS "-x objective-c++")
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        ${COCOA_FRAMEWORK}
        ${METAL_FRAMEWORK}
        ${QUARTZCORE_FRAMEWORK}
    )
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE FALSE
        XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC NO
    )
endif()
if(WIN32)
    #windows networking lib
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Ws2_32 ole32 mmdevapi)
endif()

