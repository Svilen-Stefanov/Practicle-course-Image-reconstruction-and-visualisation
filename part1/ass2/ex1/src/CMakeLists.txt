cmake_minimum_required(VERSION 3.0)

set(ASS2_EX1_SIGN_SRCS stack.cpp)
add_library(ass2_ex1_lib ${ASS2_EX1_SIGN_SRCS})

set(ASS2_EX1_SRCS main.cpp)
add_executable(ass2_ex1 ${ASS2_EX1_SRCS})
target_link_libraries(ass2_ex1 ass2_ex1_lib)

set(ASS2_EX1_TEST test.cpp)
add_executable(ass2_ex1_test ${ASS2_EX1_TEST})
target_link_libraries(ass2_ex1_test ass2_ex1_lib)
