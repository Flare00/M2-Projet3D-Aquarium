cmake_minimum_required (VERSION 3.21)

#project init
project (
        Aquarium
        VERSION 0.1
        LANGUAGES CXX
)

find_package(OpenGL REQUIRED)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}>)
set (CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/pdb)

#Subdirectories
add_subdirectory(${CMAKE_SOURCE_DIR}/aquarium/libs)

add_executable(Aquarium)

target_sources(Aquarium PUBLIC aquarium/main.cpp)

target_compile_features(Aquarium PUBLIC cxx_std_17)

target_link_libraries(Aquarium
        ${OPENGL_LIBRARY}
        libs
)

target_include_directories(Aquarium PUBLIC
        "${CMAKE_SOURCE_DIR}/aquarium/sources"
        $<TARGET_PROPERTY:libs,INTERFACE_INCLUDE_DIRECTORIES>
)

set_target_properties(Aquarium PROPERTIES OUTPUT_NAME "Aquarium" SUFFIX ".exe")