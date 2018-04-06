cmake_minimum_required(VERSION 3.0)

set(ASS2_EX4_SIGN_SRCS Rational.cpp)
add_library(ass2_ex4_lib ${ASS2_EX4_SIGN_SRCS})

set(ASS2_EX4_SRCS main.cpp)
add_executable(ass2_ex4 ${ASS2_EX4_SRCS})
target_link_libraries(ass2_ex4 ass2_ex4_lib)

set(ASS2_EX4_TEST test.cpp)
add_executable(ass2_ex4_test ${ASS2_EX4_TEST})
target_link_libraries(ass2_ex4_test ass2_ex4_lib)
