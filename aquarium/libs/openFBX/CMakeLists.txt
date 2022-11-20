cmake_minimum_required (VERSION 3.21)

#project init
project (
	open_fbx
	VERSION 1.0
	LANGUAGES CXX C
)

add_library(open_fbx miniz.c ofbx.cpp ) 

target_include_directories(open_fbx PUBLIC ./)