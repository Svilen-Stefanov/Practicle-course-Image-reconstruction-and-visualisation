cmake_minimum_required(VERSION 3.0)

set(ASS3_EX4_SIGN_SRCS File.cpp Directory.cpp)
add_library(ass3_ex4_lib ${ASS3_EX4_SIGN_SRCS})

set(ASS3_EX4_SRCS main.cpp)
add_executable(ass3_ex4 ${ASS3_EX4_SRCS})
target_link_libraries(ass3_ex4 ass3_ex4_lib)

set(ASS3_EX4_TEST test.cpp)
add_executable(ass3_ex4_test ${ASS3_EX4_TEST})
target_link_libraries(ass3_ex4_test ass3_ex4_lib)
