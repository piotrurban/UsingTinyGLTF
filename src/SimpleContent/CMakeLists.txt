cmake_minimum_required(VERSION 3.18)

project(SimpleContent)

add_executable(SimpleContentExample "simple_content_main.cpp" "SimpleContent.h" "SimpleContent.cpp")

target_link_libraries(SimpleContentExample PRIVATE ${EXEC_LIBS})
target_include_directories(SimpleContentExample PRIVATE ${EXEC_INCLUDE_DIRS})