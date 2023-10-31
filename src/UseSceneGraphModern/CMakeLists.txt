cmake_minimum_required(VERSION 3.20)

project(UseSceneGraphModernProject)

ADD_TINYGLTF_PROJECT_N(UseSceneGraphModern SOURCES "use_scene_graph_modern_main.cpp" "Handlers.h" "Handlers.cpp" "NodeVisitors.h" "NodeVisitors.cpp")

target_link_libraries ( UseSceneGraphModern PRIVATE  SimpleContent )

