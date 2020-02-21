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
add_subdirectory(lodepng)

add_executable(EndlessVoid
  src/entity/entity.cpp src/entity/entity.h
  src/entity/platform.cpp src/entity/platform.h
  src/entity/platform_stone1.cpp src/entity/platform_stone1.h
  src/entity/void_entity.cpp src/entity/void_entity.h

  src/input/input_manager.cpp src/input/input_manager.h

  src/physics/aabb.cpp src/physics/aabb.h
  src/physics/dynamic_aabb.cpp src/physics/dynamic_aabb.h
  src/physics/math.cpp src/physics/math.h
  src/physics/physics_engine.cpp src/physics/physics_engine.h

  src/render/camera.cpp src/render/camera.h
  src/render/render_engine.cpp src/render/render_engine.h
  src/render/render_entity.cpp src/render/render_entity.h
  src/render/render_loader.cpp src/render/render_loader.h
  src/render/texture_manager.cpp src/render/texture_manager.h
  src/render/wf_obj_manager.cpp src/render/wf_obj_manager.h

  src/state/state_ingame.cpp src/state/state_ingame.h
  src/state/state.h

  src/endless_void.h src/endless_void.cpp
  src/main.cpp
  src/path_generator.cpp src/path_generator.h
  src/player.cpp src/player.h
  src/world.cpp src/world.h)

set_target_properties(EndlessVoid
	PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/build")

file(COPY
  ${PROJECT_SOURCE_DIR}/resources
  DESTINATION ${PROJECT_BINARY_DIR}/build)

target_link_libraries(EndlessVoid
	OpenGL::GL
	glfw
	glad
  lodepng)

target_include_directories(EndlessVoid PUBLIC
	${OPENGL_INCLUDE_DIR}
	${PROJECT_BINARY_DIR}
	${PROJECT_BINARY_DIR}/glm/include)
