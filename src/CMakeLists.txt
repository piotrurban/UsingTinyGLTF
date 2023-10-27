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
  ${CMAKE_THREAD_LIBS_INIT}
  glm::glm
)

list(APPEND EXEC_INCLUDE_DIRS
${GLEW_INCLUDE_DIR}
${GLFW3_INCLUDE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/TinyGltfTools
)
add_compile_definitions(PROJECT_INCLUDE_DIR=${PROJECT_INCLUDE_DIR})

add_subdirectory(TinyGltfTools)
add_subdirectory(UseSceneGraph)
add_subdirectory(Tetrahedron)
add_subdirectory(UseSceneGraphModern)
