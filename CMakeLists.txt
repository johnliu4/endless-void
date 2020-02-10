cmake_minimum_required(VERSION 3.10)
project(EndlessVoid VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenGL REQUIRED)

# setup GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)
add_subdirectory(glad)

add_executable(EndlessVoid
  src/main.cpp
  src/render_loader.cpp src/render_loader.h)

set_target_properties(EndlessVoid
	PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/build")

file(COPY
  ${PROJECT_SOURCE_DIR}/resources
  DESTINATION ${PROJECT_BINARY_DIR}/build)

target_link_libraries(EndlessVoid
	OpenGL::GL
	glfw
	glad)

target_include_directories(EndlessVoid PUBLIC
	${OPENGL_INCLUDE_DIR}
	${PROJECT_BINARY_DIR}
	${PROJECT_BINARY_DIR}/glm/include)
