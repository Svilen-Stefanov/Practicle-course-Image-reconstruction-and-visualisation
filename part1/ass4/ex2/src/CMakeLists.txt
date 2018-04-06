cmake_minimum_required(VERSION 3.0)

set(ASS4_EX2_SRCS main.cpp)
add_executable(ass4_ex2 ${ASS4_EX2_SRCS})
add_library(ass4_ex2_lib ${ASS4_EX2_SRCS})

set(ASS4_EX2_TEST test.cpp)
add_executable(ass4_ex2_test ${ASS4_EX2_TEST})
target_link_libraries(ass4_ex2_test ass4_ex2_lib)
