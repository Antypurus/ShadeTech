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

if(WIN32)
# Locate Windows SDK base directory
set(WINSDK_BASE "$ENV{ProgramFiles\(x86\)}/Windows Kits/10/Include")

# Get list of SDK versions (directories in Include)
file(GLOB WINSDK_VERSIONS RELATIVE "${WINSDK_BASE}" "${WINSDK_BASE}/*")

# Sort versions in descending order to prefer the latest
list(SORT WINSDK_VERSIONS COMPARE STRING ORDER DESCENDING)

# Find the first valid SDK version with required headers
foreach(VERSION ${WINSDK_VERSIONS})
    if(EXISTS "${WINSDK_BASE}/${VERSION}/ucrt/corecrt.h")
        set(WINSDK_VERSION "${VERSION}")
        break()
    endif()
endforeach()

# Check if a valid SDK version was found
if(NOT WINSDK_VERSION)
    message(FATAL_ERROR "Could not find a valid Windows SDK version.")
endif()

# Construct include paths
find_path(WINSDK_UCRT_INCLUDE_DIR NAMES corecrt.h
    PATHS "${WINSDK_BASE}/${WINSDK_VERSION}/ucrt"
    NO_DEFAULT_PATH)
find_path(WINSDK_UM_INCLUDE_DIR NAMES windows.h
    PATHS "${WINSDK_BASE}/${WINSDK_VERSION}/um"
    NO_DEFAULT_PATH)
find_path(WINSDK_SHARED_INCLUDE_DIR NAMES winapifamily.h
    PATHS "${WINSDK_BASE}/${WINSDK_VERSION}/shared"
    NO_DEFAULT_PATH)

# Check if paths were found
if(NOT WINSDK_UCRT_INCLUDE_DIR OR NOT WINSDK_UM_INCLUDE_DIR OR NOT WINSDK_SHARED_INCLUDE_DIR)
    message(FATAL_ERROR "Could not find Windows SDK include directories for version ${WINSDK_VERSION}.")
endif()

# Add include directories
include_directories(
    ${WINSDK_UCRT_INCLUDE_DIR}
    ${WINSDK_UM_INCLUDE_DIR}
    ${WINSDK_SHARED_INCLUDE_DIR}
)
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
if(APPLE)
    find_library(COCOA_FRAMEWORK Cocoa)
    find_library(METAL_FRAMEWORK Metal)
    find_library(QUARTZCORE_FRAMEWORK QuartzCore)
endif()

file(GLOB_RECURSE SHD_GLOBAL_MODULE_SOURCES CONFIGURE_DEPENDS
    ShadeTech/**.cpp
    ShadeTech/**.h)
file(GLOB_RECURSE SHD_OBJC_SOURCES CONFIGURE_DEPENDS ShadeTech/**.mm)
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

add_executable(${CMAKE_PROJECT_NAME} ${SHD_MODULE_SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ./ShadeTech/)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glfw ${GLFW_LIBRARIES} cpptrace::cpptrace)
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
        XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
    )
endif()
if(WIN32)
    #windows networking lib
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Ws2_32 ole32 mmdevapi)
endif()
