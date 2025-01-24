cmake_minimum_required(VERSION 3.28)
project(ShadeTech LANGUAGES C CXX)

find_program(DSYMUTIL_PROGRAM dsymutil)
if (DSYMUTIL_PROGRAM)
  foreach(lang C CXX)
    foreach(var LINK_EXECUTABLE CREATE_SHARED_LIBRARY)
      set(CMAKE_${lang}_${var} "${CMAKE_${lang}_${var}}" "${DSYMUTIL_PROGRAM} <TARGET>")
    endforeach()
  endforeach()
endif()

# C++ Basic Configuration
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR})

# CMake configs
include(CMake/Warnings.cmake)
include(CMake/Platform.cmake)

# third party dependencies
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(ThirdParty/GLFW EXCLUDE_FROM_ALL)
add_subdirectory(ThirdParty/Catch2 EXCLUDE_FROM_ALL)

if(NOT WIN32)
    set(CPPTRACE_GET_SYMBOLS_WITH_ADDR2LINE ON)
endif()
add_subdirectory(ThirdParty/cpptrace)

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

file(GLOB_RECURSE SHD_GLOBAL_MODULE_SOURCES CONFIGURE_DEPENDS
    ShadeTech/**.cxx)
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
elseif(UNIX)
    list(APPEND SHD_MODULE_SOURCES ${SHD_LINUX_SOURCES})
    list(APPEND SHD_MODULE_SOURCES ${SHD_POSIX_SOURCES})
endif()

add_library(modules)
target_sources(modules
        PUBLIC
        FILE_SET CXX_MODULES FILES
        ${SHD_MODULE_SOURCES}
)
target_include_directories(modules PRIVATE
    ./ShadeTech/
)
target_link_libraries(modules PRIVATE glfw ${GLFW_LIBRARIES} cpptrace::cpptrace)
configure_compiler_warnings(modules)
setup_vulkan_dependency(modules)

if(WIN32)
    #windows networking lib
    target_link_libraries(modules PRIVATE Ws2_32)
endif()

add_executable(${CMAKE_PROJECT_NAME} ShadeTech/main.cpp)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ./ShadeTech/)
target_link_libraries(${CMAKE_PROJECT_NAME} modules)
