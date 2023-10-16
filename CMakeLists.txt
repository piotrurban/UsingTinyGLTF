cmake_minimum_required(VERSION 3.20)


project(Cubes)
list (APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" )
message(STATUS "Prefix Path: ${CMAKE_PREFIX_PATH}")

add_subdirectory(src)

