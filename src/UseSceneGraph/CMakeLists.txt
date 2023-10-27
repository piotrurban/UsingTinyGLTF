cmake_minimum_required(VERSION 3.20)

project(UseSceneGraphProject)

add_executable(UseSceneGraph "use_scene_graph_main.cpp")

target_include_directories(UseSceneGraph PRIVATE ${EXEC_INCLUDE_DIRS}
glm::glm)

target_link_libraries ( UseSceneGraph
  TinyGltfTools
  ${EXEC_LIBS}
)
add_dependencies(UseSceneGraph CopyModels glm::glm)