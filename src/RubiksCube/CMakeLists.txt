cmake_minimum_required(VERSION 3.18)

project(RubiksCube)

add_executable(RubiksCube "rubiks_cube_main.cpp" ${MODEL_HEADERS})

target_link_libraries(RubiksCube PRIVATE ${EXEC_LIBS})
target_include_directories(RubiksCube PRIVATE ${EXEC_INCLUDE_DIRS})

add_dependencies(RubiksCube CopyModels)