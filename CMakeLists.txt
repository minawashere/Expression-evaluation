cmake_minimum_required(VERSION 3.31)
project(expression_evaluation C)

set(CMAKE_C_STANDARD 17)

add_executable(expression_evaluation main.c)
target_link_libraries(expression_evaluation  m)