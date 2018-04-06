cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories("include")
file(GLOB SRC_FILES "src/*.cpp")
file(GLOB HDR_FILES "include/*.h")
file(GLOB IMG_FILES "imageFiles.qrc")

find_package(Qt5Widgets REQUIRED)
QT5_ADD_RESOURCES (RSRC_SOURCES ${IMG_FILES})

add_library(ass5_ex2_lib ${SRC_FILES} ${HDR_FILES})
target_link_libraries(ass5_ex2_lib Qt5::Widgets)

add_executable(ass5_ex2_bin ${SRC_FILES} ${HDR_FILES} ${RSRC_SOURCES})
target_link_libraries(ass5_ex2_bin Qt5::Widgets)
