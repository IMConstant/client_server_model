cmake_minimum_required(VERSION 3.15)

project(untitled1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

add_subdirectory(./dependencies/glew/build/cmake)
add_subdirectory(./dependencies/glfw)
add_subdirectory(./dependencies/imgui/imgui-master)

add_subdirectory(client)
add_subdirectory(server)