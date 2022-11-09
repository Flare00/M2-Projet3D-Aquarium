cmake_minimum_required (VERSION 3.21)

add_library(libs INTERFACE)
#Subdirectories

add_subdirectory(glew-2.1.0/build/cmake)
add_subdirectory(glfw-3.3.8)
add_subdirectory(glm-0.9.9.8)
add_subdirectory(stb)

target_link_libraries(libs INTERFACE stb glm glfw glew)