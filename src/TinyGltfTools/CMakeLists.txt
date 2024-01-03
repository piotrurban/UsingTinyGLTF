cmake_minimum_required(VERSION 3.20)

add_library(TinyGltfTools STATIC 
"trackball.cc"
"trackball.h"
"TransformUtils.h"
"TransformUtils.cpp"
"BallTracker.h"
"BallTracker.cpp"
"Content.h"
"Content.cpp"
"content_drawing.h"
"content_drawing.cpp"
"tinygltf_implementation.cpp"
"utils.h"
"utils.cpp"
 "get_mesh_data.cpp" "get_mesh_data.h" "ContentUtils.cpp" "ContentUtils.h" "TraverseContent.h" "TraverseContent.cpp" "GLMUtils.h" "GLMUtils.cpp" "GLFWUtils.h" "GLFWUtils.cpp")
target_include_directories(TinyGltfTools PRIVATE
${GLEW_INCLUDE_DIR}
${GLFW3_INCLUDE_DIR}
${PROJECT_INCLUDE_DIR}
${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(TinyGltfTools PRIVATE glm::glm)