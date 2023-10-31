cmake_minimum_required(VERSION 3.18)

project(RubiksCube)

ADD_TINYGLTF_PROJECT_N(RubiksCube SOURCES "rubiks_cube_main.cpp" "test.h" ${MODEL_HEADERS})
