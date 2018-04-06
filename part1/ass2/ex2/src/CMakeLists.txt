cmake_minimum_required(VERSION 3.0)

set(ASS2_EX2_SIGN_SRCS NamePairs.cpp)
add_library(ass2_ex2_lib ${ASS2_EX2_SIGN_SRCS})

set(ASS2_EX2_SRCS main.cpp)
add_executable(ass2_ex2 ${ASS2_EX2_SRCS})
target_link_libraries(ass2_ex2 ass2_ex2_lib)

set(ASS2_EX2_TEST test.cpp)
add_executable(ass2_ex2_test ${ASS2_EX2_TEST})
target_link_libraries(ass2_ex2_test ass2_ex2_lib)
