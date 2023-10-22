cmake_minimum_required(VERSION 3.20)


project(Cubes)
list (APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" )
message(STATUS "Prefix Path: ${CMAKE_PREFIX_PATH}")

set(CMAKE_CXX_STANDARD 20)
add_compile_options("/Zc:__cplusplus")


add_subdirectory(CopyModels)
add_subdirectory(src)

