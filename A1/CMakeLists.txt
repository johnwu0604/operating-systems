cmake_minimum_required(VERSION 3.6)
project(A1)

set(CMAKE_C_STANDARD 99)

set(PART_1 a1_redirect.c)
set(PART_2 a1_command_piping.c)
add_executable(PART_1 ${PART_1})
add_executable(PART_2 ${PART_2})