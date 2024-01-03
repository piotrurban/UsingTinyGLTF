cmake_minimum_required(VERSION 3.18)

set (IMGUI_DIR "C:/Apps/Imgui")
file (GLOB IMGUI_SOURCES "${IMGUI_DIR}/*.h" "${IMGUI_DIR}/*.cpp" "${IMGUI_DIR}/backends/imgui_impl_opengl3.*" "${IMGUI_DIR}/backends/imgui_impl_glfw.*")
add_library(ImGui ${IMGUI_SOURCES})
target_include_directories(ImGui PUBLIC ${IMGUI_DIR} ${GLFW3_INCLUDE_DIR})