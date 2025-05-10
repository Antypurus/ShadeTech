add_executable(${CMAKE_PROJECT_NAME}-client client.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME}-client PRIVATE ${CMAKE_PROJECT_NAME})
