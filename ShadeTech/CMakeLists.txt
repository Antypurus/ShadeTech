file(GLOB_RECURSE SHD_CORE_SOURCES CONFIGURE_DEPENDS
    **.cpp
    **.h)

add_executable(main ${SHD_CORE_SOURCES})
target_include_directories(main PRIVATE .)

target_compile_options(main PRIVATE -nostdinc++)
target_link_options(main PRIVATE -nostdlib++)
