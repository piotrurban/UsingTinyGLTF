cmake_minimum_required(VERSION 3.18)

project(RubiksCube)

ADD_TINYGLTF_PROJECT(RubiksCube "rubiks_cube_main.cpp" ${MODEL_HEADERS})
