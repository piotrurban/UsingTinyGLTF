cmake_minimum_required(VERSION 3.20)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/generated_models.txt"
COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/model" "${CMAKE_CURRENT_BINARY_DIR}/generated_model"
COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_BINARY_DIR}/generated_models.txt"
DEPENDS "${CMAKE_SOURCE_DIR}/model"
)

add_custom_target(CopyModels ALL
DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/generated_models.txt"
)