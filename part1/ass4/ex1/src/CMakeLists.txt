cmake_minimum_required(VERSION 3.0)

set(ASS4_EX1_SRCS main.cpp)
add_executable(ass4_ex1 ${ASS4_EX1_SRCS})
add_library(ass4_ex1_lib ${ASS4_EX1_SRCS})

set(ASS4_EX1_TEST test.cpp)
add_executable(ass4_ex1_test ${ASS4_EX1_TEST})
target_link_libraries(ass4_ex1_test ass4_ex1_lib)
