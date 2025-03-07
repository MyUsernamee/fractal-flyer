cmake_minimum_required(VERSION 3.28)
project(fractal-flyer)


set(SHADER_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(COMMON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/common")
set(SHADER_DST_DIR "${CMAKE_CURRENT_BINARY_DIR}/res")

function(watch)
    set_property(
            DIRECTORY
            APPEND
            PROPERTY CMAKE_CONFIGURE_DEPENDS ${ARGV}
    )
endfunction()

include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

file(MAKE_DIRECTORY ${SHADER_DST_DIR})

file(GLOB_RECURSE SHADER_FILES "${SHADER_SRC_DIR}/*.glsl")
file(GLOB_RECURSE COMMON_FILES "${COMMON_DIR}/*.h")

set(PROCESSED_SHADERS)

foreach(shader_file ${SHADER_FILES})
foreach (common_file ${COMMON_FILES})
	get_filename_component(shader_name ${shader_file} NAME)
	set(output_file "${SHADER_DST_DIR}/${shader_name}")

	add_custom_command(
		OUTPUT ${output_file}
		COMMAND ${CMAKE_COMMAND} -E env python3 ${CMAKE_CURRENT_SOURCE_DIR}/scripts/preprocess_shaders.py 
		${shader_file} ${output_file} ${COMMON_DIR}
		DEPENDS "${shader_file};${common_file}"
		COMMENT "Preprocessing shader ${shader_name}"
	)
		
	list(APPEND PROCESSED_SHADERS ${output_file})
endforeach()
endforeach()




# Explicitly add target
add_custom_target(process_shaders ALL DEPENDS ${PROCESSED_SHADERS})

file(GLOB_RECURSE SOURCES "./src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE common/)
target_link_libraries(${PROJECT_NAME} raylib glm::glm)

add_dependencies(${PROJECT_NAME} process_shaders)