cmake_minimum_required(VERSION 3.20)

add_library(TinyGltfTools STATIC "trackball.cc" "trackball.h"  "TransformUtils.h" "TransformUtils.cpp")
target_include_directories(TinyGltfTools PRIVATE ${GLEW_INCLUDE_DIR} ${GLFW3_INCLUDE_DIR} ${PROJECT_INCLUDE_DIR})