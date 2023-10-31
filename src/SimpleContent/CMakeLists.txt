cmake_minimum_required(VERSION 3.18)

project(SimpleContent)

add_library(SimpleContent STATIC "SimpleContent.h" "SimpleContent.cpp" "SimpleContentFactory.h"  "SimpleContentFactory.cpp")

target_link_libraries(SimpleContent PRIVATE ${EXEC_LIBS})
target_include_directories(SimpleContent PRIVATE ${EXEC_INCLUDE_DIRS})

ADD_TINYGLTF_PROJECT(SimpleContentExample "simple_content_main.cpp" )