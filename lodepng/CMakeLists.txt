cmake_minimum_required(VERSION 3.10)
project(Lodepng)

add_library(lodepng include/lodepng.h lodepng.cpp)
target_include_directories(lodepng PUBLIC include)
