cmake_minimum_required (VERSION 3.21)

#project init
project (
	stb
	VERSION 1.0
	LANGUAGES CXX
)

add_library(stb INTERFACE) 
target_include_directories(stb INTERFACE .) 