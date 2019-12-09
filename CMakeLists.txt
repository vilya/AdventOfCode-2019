cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(advent-of-code-2019 VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)

include_directories(intcode)

add_library(intcode intcode/intcode.cpp intcode/intcode.h)

add_executable(day03a day03/day03a.cpp)
add_executable(day03b day03/day03b.cpp)

add_executable(day04a day04/day04a.cpp)
add_executable(day04b day04/day04b.cpp)

add_executable(day05a day05/day05a.cpp)
target_link_libraries(day05a intcode)

add_executable(day05b day05/day05b.cpp)
target_link_libraries(day05b intcode)

add_executable(day06a day06/day06a.cpp)
add_executable(day06b day06/day06b.cpp)

add_executable(day07a day07/day07a.cpp)
target_link_libraries(day07a intcode)

add_executable(day07b day07/day07b.cpp)
target_link_libraries(day07b intcode)

add_executable(day08a day08/day08a.cpp)
add_executable(day08b day08/day08b.cpp)

add_executable(day09a day09/day09a.cpp)
target_link_libraries(day09a intcode)

add_executable(day09b day09/day09b.cpp)
target_link_libraries(day09b intcode)
