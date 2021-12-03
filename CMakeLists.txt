cmake_minimum_required(VERSION 3.20)
project(AoC_2021)

set(CMAKE_CXX_STANDARD 17)

configure_file(input_1 input_1 COPYONLY)
add_executable(AoC_2021_day_1 day_1.cpp)

configure_file(input_2 input_2 COPYONLY)
add_executable(AoC_2021_day_2 day_2.cpp)

configure_file(input_3 input_3 COPYONLY)
add_executable(AoC_2021_day_3 day_3.cpp)