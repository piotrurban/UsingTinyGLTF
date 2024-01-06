cmake_minimum_required(VERSION 3.18)

ADD_TINYGLTF_PROJECT_N(ShaderToy SOURCES shader_toy.cpp)
target_link_libraries(ShaderToy PRIVATE SimpleContent ImGui ShaderUtils)
