cmake_minimum_required(VERSION 3.0)

set(ASS3_EX3_SIGN_SRCS Tracer.cpp)
add_library(ass3_ex3_lib ${ASS3_EX3_SIGN_SRCS})

set(ASS3_EX3_SRCS main.cpp)
add_executable(ass3_ex3 ${ASS3_EX3_SRCS})
target_link_libraries(ass3_ex3 ass3_ex3_lib)

set(ASS3_EX3_TEST test.cpp)
add_executable(ass3_ex3_test ${ASS3_EX3_TEST})
target_link_libraries(ass3_ex3_test ass3_ex3_lib)
