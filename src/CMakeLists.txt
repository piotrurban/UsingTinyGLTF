cmake_minimum_required(VERSION 3.20)

set ( CMAKE_PREFIX_PATH cmake )

include_directories([=[C:\Apps\tinygltf-release]=])

find_package(GLFW3 REQUIRED)
find_package(OpenGL REQUIRED)
set(GLEW_INCLUDE_DIR "C:\\Apps\\glew-2.2.0\\include")
set(GLEW_LIBRARIES [=[C:\Apps\glew-2.2.0\lib\Debug\x64\glew32d.lib]=])

add_executable(Cubes
cubes_main.cpp
content_creation.h
content_creation.cpp
utils.h
utils.cpp
)

target_include_directories(Cubes PRIVATE ${GLEW_INCLUDE_DIR} ${GLFW3_INCLUDE_DIR})
target_link_libraries ( Cubes
  ${GLEW_LIBRARIES}
  ${GLFW3_glfw_LIBRARY}
  ${OPENGL_gl_LIBRARY}
  ${OPENGL_glu_LIBRARY}
  ${CMAKE_DL_LIBS}
  ${CMAKE_THREAD_LIBS_INIT}
  )
