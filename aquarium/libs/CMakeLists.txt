cmake_minimum_required (VERSION 3.21)

add_library(libs INTERFACE)
#Subdirectories

add_subdirectory(glew-2.2.0/build/cmake)
add_subdirectory(glfw-3.3.8)
add_subdirectory(glm-0.9.9.8)
add_subdirectory(stb)

target_link_libraries(libs INTERFACE stb glm glfw glew)

target_include_directories(libs INTERFACE
        $<TARGET_PROPERTY:stb,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:glm,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:glfw,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:glew,INCLUDE_DIRECTORIES>
)
