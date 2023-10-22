cmake_minimum_required(VERSION 3.20)

add_executable(Tetrahedron "cubes_main.cpp")

target_include_directories(Tetrahedron PRIVATE ${EXEC_INCLUDE_DIRS})
target_link_libraries ( Tetrahedron
  TinyGltfTools
  ${EXEC_LIBS}
)

