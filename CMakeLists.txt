cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
	glfw 
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG master
)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
	assimp
	GIT_REPOSITORY https://github.com/assimp/assimp.git
	GIT_TAG master
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(assimp)


FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

project(glfw-app)

include_directories(include)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME} src/main.cpp src/graphics/window.cpp src/graphics/texture.cpp src/graphics/camera.cpp src/graphics/shader.cpp src/graphics/color.cpp src/glad.c)
target_link_libraries(${PROJECT_NAME} glm::glm glfw assimp)
