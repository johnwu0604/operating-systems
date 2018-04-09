cmake_minimum_required(VERSION 3.9)
project(A3 C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(Q2 260612056_part2.c)
add_executable(Q3A resource_request_simulator.c)