

function(ADD_TINYGLTF_PROJECT_N project_name)
	cmake_parse_arguments(ADD_PROJECT "" "" "SOURCES" ${ARGN})
	message(STATUS "_N: Adding project ${project_name} with sources ${ADD_PROJECT_SOURCES}")
	add_executable(${project_name} ${ADD_PROJECT_SOURCES})
	target_link_libraries(${project_name} PRIVATE ${EXEC_LIBS})
	target_include_directories(${project_name} PRIVATE ${EXEC_INCLUDE_DIRS})
	add_dependencies(${project_name} CopyModels)
endfunction()