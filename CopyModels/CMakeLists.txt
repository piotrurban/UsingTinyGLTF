cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE MODEL_FILES "${CMAKE_SOURCE_DIR}/model/**")
message(STATUS "MODEL FILES: ${MODEL_FILES}")

set(FILE_LIST_PATH "${CMAKE_CURRENT_BINARY_DIR}/model_file_list.txt")
add_custom_command(OUTPUT always_rebuild
COMMAND ${CMAKE_COMMAND} -DSRC_DIR=${CMAKE_SOURCE_DIR} -DOUTPUT="${FILE_LIST_PATH}" -P "${CMAKE_CURRENT_SOURCE_DIR}/GenerateModelFileList.cmake"
COMMENT "Generating model file list from: ${CMAKE_SOURCE_DIR}, ${CMAKE_CURRENT_SOURCE_DIR} "
)

add_custom_target(GenerateModelFileList ALL 
DEPENDS always_rebuild)

add_custom_command(OUTPUT "${FILE_LIST_PATH}"
COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/model" "${CMAKE_BINARY_DIR}/generated_model"
COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_BINARY_DIR}/generated_models.txt"
COMMAND ${CMAKE_COMMAND} -E echo "copying model files to ${CMAKE_BINARY_DIR}/generated_model"
DEPENDS GenerateModelFileList "${MODEL_FILES}"
)

add_custom_target(CopyModels ALL
DEPENDS "${FILE_LIST_PATH}")


file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/generated_model" NATIVE_BINARY_MODEL_DIR)
list(APPEND MODEL_HEADERS "${CMAKE_BINARY_DIR}/generated_model/models.h")
configure_file("models.h.in" "${CMAKE_BINARY_DIR}/generated_model/models.h")

