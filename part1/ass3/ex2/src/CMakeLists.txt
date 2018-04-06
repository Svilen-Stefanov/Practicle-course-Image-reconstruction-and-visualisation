cmake_minimum_required(VERSION 3.0)

set(ASS3_EX2_SIGN_SRCS VectorIterator.cpp ListIterator.cpp)
add_library(ass3_ex2_lib ${ASS3_EX2_SIGN_SRCS})

set(ASS3_EX2_SRCS main.cpp)
add_executable(ass3_ex2 ${ASS3_EX2_SRCS})
target_link_libraries(ass3_ex2 ass3_ex2_lib)

set(ASS3_EX2_TEST test.cpp)
add_executable(ass3_ex2_test ${ASS3_EX2_TEST})
target_link_libraries(ass3_ex2_test ass3_ex2_lib)
