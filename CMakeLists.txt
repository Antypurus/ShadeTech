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

set(CPPTRACE_GET_SYMBOLS_WITH_ADDR2LINE ON)
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

file(GLOB_RECURSE SHD_MODULE_SOURCES CONFIGURE_DEPENDS
    ShadeTech/**.cxx)
add_library(modules)
target_sources(modules
        PUBLIC
        FILE_SET CXX_MODULES FILES
        ${SHD_MODULE_SOURCES}
)
target_include_directories(modules PRIVATE
    ./ShadeTech/
)
target_link_libraries(modules PRIVATE glfw ${GLFW_LIBRARIES})
configure_compiler_warnings(modules)
setup_vulkan_dependency(modules)

add_executable(${CMAKE_PROJECT_NAME} ShadeTech/ShadeTech.cpp)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ./ShadeTech/)
target_link_libraries(${CMAKE_PROJECT_NAME} modules cpptrace::cpptrace)
if(WIN32)
    target_link_libraries(${CMAKE_PROJECT_NAME} dbghelp.lib)
else()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g2")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g2")

    find_library(DWARF_LIB dwarf)
    find_path(DWARF_HEADERS NAMES dwarf.h PATH_SUFFIXES libdwarf libdwarf-0)
    target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${DWARF_HEADERS})
    target_link_libraries(${CMAKE_PROJECT_NAME} ${DWARF_LIB})
endif()
