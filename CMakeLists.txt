cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

project(glfw-app)

include_directories(include)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME} src/main.cpp src/graphics/window.cpp src/graphics/texture.cpp src/graphics/camera.cpp src/graphics/shader.cpp src/graphics/color.cpp src/glad.c)
target_link_libraries(${PROJECT_NAME} glfw assimp)
