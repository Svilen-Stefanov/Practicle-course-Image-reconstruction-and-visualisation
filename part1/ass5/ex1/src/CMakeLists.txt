cmake_minimum_required(VERSION 3.0)

set(ASS5_EX1_SIGN_SRCS Tomograph.cpp)
add_library(ass5_ex1_lib ${ASS5_EX1_SIGN_SRCS})

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

set(ASS5_EX1_SRCS main.cpp)
add_executable(ass5_ex1 ${ASS5_EX1_SRCS})
target_link_libraries(ass5_ex1 ass5_ex1_lib)
