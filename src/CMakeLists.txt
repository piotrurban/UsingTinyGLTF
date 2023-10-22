cmake_minimum_required(VERSION 3.20)

include_directories([=[C:\Apps\tinygltf-release]=])
message(STATUS "in src current_list_dir: ${CMAKE_CURRENT_LIST_DIR}")
message(STATUS "in src current_source_dir: ${CMAKE_CURRENT_SOURCE_DIR}")
set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
find_package(GLFW3 REQUIRED)
find_package(OpenGL REQUIRED)
set(GLEW_INCLUDE_DIR "C:\\Apps\\glew-2.2.0\\include")
set(GLEW_LIBRARIES [=[C:\Apps\glew-2.2.0\lib\Debug\x64\glew32d.lib]=])

list(APPEND EXEC_LIBS  ${GLEW_LIBRARIES}
  ${GLFW3_glfw_LIBRARY}
  ${OPENGL_gl_LIBRARY}
  ${OPENGL_glu_LIBRARY}
  ${CMAKE_DL_LIBS}
  ${CMAKE_THREAD_LIBS_INIT})

list(APPEND EXEC_INCLUDE_DIRS
${GLEW_INCLUDE_DIR}
${GLFW3_INCLUDE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(TinyGltfTools)
add_subdirectory(UseSceneGraph)
add_subdirectory(Tetrahedron)

#add_executable(Cubes
#"cubes_main.cpp"
#"content_drawing.h"
#"content_drawing.cpp"
#"utils.h"
#"utils.cpp"
#"Content.h"
#"Content.cpp"
#"gl_includes.h"
# "tinygltf_implementation.cpp"
# "TinyGltfTools/BallTracker.cpp"
# "TinyGltfTools/BallTracker.h"
# "UseSceneGraph/use_scene_graph_main.cpp")
#
#target_include_directories(Cubes PRIVATE ${GLEW_INCLUDE_DIR} ${GLFW3_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
#target_link_libraries ( Cubes
#  TinyGltfTools
#  ${GLEW_LIBRARIES}
#  ${GLFW3_glfw_LIBRARY}
#  ${OPENGL_gl_LIBRARY}
#  ${OPENGL_glu_LIBRARY}
#  ${CMAKE_DL_LIBS}
#  ${CMAKE_THREAD_LIBS_INIT}
#  )
#