cmake_minimum_required(VERSION 3.0)

set(ASS2_EX3_SIGN_SRCS namePairs.cpp)
add_library(ass2_ex3_lib ${ASS2_EX3_SIGN_SRCS})

set(ASS2_EX3_SRCS main.cpp)
add_executable(ass2_ex3 ${ASS2_EX3_SRCS})
target_link_libraries(ass2_ex3 ass2_ex3_lib)

set(ASS2_EX3_TEST test.cpp)
add_executable(ass2_ex3_test ${ASS2_EX3_TEST})
target_link_libraries(ass2_ex3_test ass2_ex3_lib)
