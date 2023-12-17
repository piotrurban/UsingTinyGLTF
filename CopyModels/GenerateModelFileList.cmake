cmake_minimum_required(VERSION 3.15)

file(GLOB_RECURSE FILE_LIST "${SRC_DIR}/model/**")
message(STATUS "FILE_LIST: ${FILE_LIST}") 
message(STATUS "SRC_DIR: ${SRC_DIR}") 

file(WRITE "${OUTPUT}" "${FILE_LIST}")

