cmake_minimum_required(VERSION 3.18)

project(SimpleContent)

add_library(SimpleContent STATIC "SimpleContent.h" "SimpleContent.cpp" "SimpleContentFactory.h"  "SimpleContentFactory.cpp")
add_executable(SimpleContentExample "simple_content_main.cpp" )

target_link_libraries(SimpleContent PRIVATE ${EXEC_LIBS})
target_include_directories(SimpleContent PRIVATE ${EXEC_INCLUDE_DIRS})

target_link_libraries(SimpleContentExample PRIVATE ${EXEC_LIBS} SimpleContent)
target_include_directories(SimpleContentExample PRIVATE ${EXEC_INCLUDE_DIRS})