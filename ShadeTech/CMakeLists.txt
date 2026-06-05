add_executable(main main.cpp)
target_compile_options(main PRIVATE -nostdinc++)
target_link_options(main PRIVATE -nostdlib++)
