cmake_minimum_required(VERSION 3.18)

add_library(ShaderUtils STATIC shader_control.h shader_control.cpp)
target_link_libraries(ShaderUtils PRIVATE ImGui SimpleContent glm::glm)
target_include_directories(ShaderUtils PUBLIC ${CMAKE_CURRENT_LIST_DIR})