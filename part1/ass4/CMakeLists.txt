cmake_minimum_required(VERSION 3.5.1)

add_subdirectory(ex1)
add_subdirectory(ex2)

# Little trick to make the header files show up in some IDEs
FILE(GLOB_RECURSE HppFiles "*.hpp")
add_custom_target(headers SOURCES ${HppFiles})


