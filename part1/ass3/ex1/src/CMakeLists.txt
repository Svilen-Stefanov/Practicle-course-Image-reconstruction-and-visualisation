cmake_minimum_required(VERSION 3.0)

set(ASS3_EX1_SIGN_SRCS MemoryOrder.cpp)
add_library(ass3_ex1_lib ${ASS3_EX1_SIGN_SRCS})

set(ASS3_EX1_SRCS main.cpp)
add_executable(ass3_ex1 ${ASS3_EX1_SRCS})
target_link_libraries(ass3_ex1 ass3_ex1_lib)

set(ASS3_EX1_TEST test.cpp)
add_executable(ass3_ex1_test ${ASS3_EX1_TEST})
target_link_libraries(ass3_ex1_test ass3_ex1_lib)
