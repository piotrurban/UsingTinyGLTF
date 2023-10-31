cmake_minimum_required(VERSION 3.20)

file(GLOB MODEL_FILES "${CMAKE_SOURCE_DIR}/model/*")
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/generated_models.txt"
COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/model" "${CMAKE_CURRENT_BINARY_DIR}/generated_model"
COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_BINARY_DIR}/generated_models.txt"
DEPENDS "${MODEL_FILES}"
)

execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/model" "${CMAKE_BINARY_DIR}/generated_model")

file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/generated_model" NATIVE_BINARY_MODEL_DIR)
list(APPEND MODEL_HEADERS "${CMAKE_BINARY_DIR}/generated_model/models.h")
configure_file("models.h.in" "${CMAKE_BINARY_DIR}/generated_model/models.h")

add_custom_target(CopyModels ALL
DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/generated_models.txt"
)

