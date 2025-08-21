cmake_minimum_required(VERSION 3.10)
project(ram_visualizer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
add_executable(ram_visualizer 
		src/main.cpp
		src/parse.cpp
		src/fetch.cpp)
