cmake_minimum_required(VERSION 3.20)

project(UseSceneGraphModernProject)

add_executable(UseSceneGraphModern "use_scene_graph_modern_main.cpp")

target_include_directories(UseSceneGraphModern PRIVATE ${EXEC_INCLUDE_DIRS}
glm::glm)

target_link_libraries ( UseSceneGraphModern
  TinyGltfTools
  ${EXEC_LIBS}
)
add_dependencies(UseSceneGraphModern CopyModels glm::glm)