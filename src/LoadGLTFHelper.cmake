list (APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" )
message(VERBOSE "Prefix Path: ${CMAKE_PREFIX_PATH}")
set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

find_package(glm REQUIRED)
message(VERBOSE "in src current_list_dir: ${CMAKE_CURRENT_LIST_DIR}")
message(VERBOSE "in src current_source_dir: ${CMAKE_CURRENT_SOURCE_DIR}")
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
  TinyGltfTools
  glm::glm
)

list(APPEND EXEC_INCLUDE_DIRS
${GLEW_INCLUDE_DIR}
${GLFW3_INCLUDE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/TinyGltfTools
${CMAKE_BINARY_DIR}
)