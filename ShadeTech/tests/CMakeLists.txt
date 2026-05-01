add_executable(ShadeTech-tests
    dynarray_tests.cpp
)
target_link_libraries(ShadeTech-tests PRIVATE
    ShadeTech
    Catch2::Catch2WithMain
)
target_include_directories(ShadeTech-tests PRIVATE
    ${CMAKE_SOURCE_DIR}/ShadeTech
)
include(CTest)
include(Catch)
catch_discover_tests(ShadeTech-tests)
