cmake_minimum_required(VERSION 3.0)
project(assignment_one)

set(ASS1_SIGN_SRCS sign.cpp)
add_library(ass1_ex8_lib ${ASS1_SIGN_SRCS})

set(ASS1_SRCS main.cpp)
add_executable(ass1_ex8 ${ASS1_SRCS})
target_link_libraries(ass1_ex8 ass1_ex8_lib)