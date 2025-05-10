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
#enable simd and other features
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # using Clang
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -march=native")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GCC
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -march=native")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using Visual Studio C++
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /arch:AVX2")
endif()

# CMake configs
include(CMake/Warnings.cmake)
include(CMake/Platform.cmake)
include(CMake/Vulkan.cmake)

# third party dependencies
if(NOT WIN32)
    set(CPPTRACE_GET_SYMBOLS_WITH_ADDR2LINE ON)
endif()
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(ThirdParty/cpptrace)
add_subdirectory(ThirdParty/GLFW EXCLUDE_FROM_ALL)
add_subdirectory(ThirdParty/Catch2 EXCLUDE_FROM_ALL)
add_subdirectory(ShadeTech/core)
add_subdirectory(ShadeTech/client)
