cmake_minimum_required(VERSION 3.5.1)

project("Part2")

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

# Entry point
set(PART2_SOURCES "src/main.cpp")
set(PART2_HEADERS "")

# All Files

set(PART2_MATH_SOURCES
	"src/math/Volume.cpp"
	"src/math/AcquisitionPose.cpp"
	"src/math/AcquisitionPoses.cpp"
	"src/math/Raytracer.cpp"
	"src/math/Detector.cpp"
	"src/math/Reconstruction.cpp"
	"src/math/SystemMatrix.cpp"
	"src/math/RegularizedSystemMatrix.cpp"
	"src/math/ConjugateGradient.cpp"
	"src/math/Camera.cpp"
	"src/math/VolumeRenderer.cpp"
	"src/math/SliceHelper.cpp"
)
set(PART2_MATH_HEADERS
	"include/math/Volume.h"
	"include/math/AcquisitionPose.h"
	"include/math/AcquisitionPoses.h"
	"include/math/Raytracer.h"
	"include/math/Detector.h"
	"include/math/ConjugateGradient.h"
	"include/math/IMatrix.h"
	"include/math/SystemMatrix.h"
	"include/math/Reconstruction.h"
	"include/math/RegularizedSystemMatrix.h"
	"include/math/Camera.h"
	"include/math/ITransferFunction.h"
	"include/math/VolumeRenderer.h"
	"include/math/SliceHelper.h"
)
set(PART2_UI_SOURCES
	"src/gui/MainWindow.cpp"
	"src/gui/AcquisitionWidget.cpp"
	"src/gui/PoseWidget.cpp"
	"src/gui/RealtimePreviewWidget.cpp"
	"src/gui/ReconstructionWidget.cpp"
	"src/gui/SliceWidget.cpp"
	"src/gui/VisualizationWidget.cpp"
	"src/gui/TransferFunctionWidget.cpp"
	"src/gui/VolumeRendererWidget.cpp"
)
set(PART2_UI_HEADERS
	"include/gui/MainWindow.h"
	"include/gui/AcquisitionWidget.h"
	"include/gui/PoseWidget.h"
	"include/gui/RealtimePreviewWidget.h"
	"include/gui/ReconstructionWidget.h"
	"include/gui/SliceWidget.h"
	"include/gui/VisualizationWidget.h"
	"include/gui/TransferFunctionWidget.h"
	"include/gui/VolumeRendererWidget.h"
)

set(PART2_IO_SOURCES "src/io/EDFhandler.cpp")
set(PART2_IO_HEADERS "include/io/EDFhandler.h" "include/io/IOutilities.h")

include_directories(part2_bin PUBLIC ./include)

# Test
set(PART2_TEST_HEADERS
	"test/catch.hpp"
	"test/TestUtils.h"
	"test/TestMatrix.h"
)
set(PART2_TEST_SOURCES
	"test/main.cpp"
	"test/AcquisitionTests.cpp"
	"test/AcRaytracerTests.cpp"
	"test/ConjugateGradientTests.cpp"
	"test/TestMatrix.cpp"
	"test/SystemMatrixTests.cpp"
	"test/CameraTests.cpp"
	"test/VolumeRendererTests.cpp"
)

# Eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Concurrent REQUIRED)
link_libraries(Qt5::Widgets)
link_libraries(Qt5::Concurrent)

# OpenMP
FIND_PACKAGE( OpenMP QUIET)
if(OPENMP_FOUND)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# Library with all the code
set(PART2_LIB_SOURCES
	${PART2_IO_SOURCES} ${PART2_MATH_SOURCES} ${PART2_UI_SOURCES}
	${PART2_IO_HEADERS} ${PART2_MATH_HEADERS} ${PART2_UI_HEADERS})
add_library(part2_lib ${PART2_LIB_SOURCES})
# Write project structure to filters
foreach(source IN LISTS PART2_LIB_SOURCES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc_1 "${source_path}")
	string(REPLACE "src" "Source Files" source_path_msvc_2 "${source_path_msvc_1}")
	string(REPLACE "include" "Headers" source_path_msvc_3 "${source_path_msvc_2}")
    source_group("${source_path_msvc_3}" FILES "${source}")
endforeach()

# Executable
add_executable(part2_bin ${PART2_SOURCES} ${PART2_HEADERS})
target_link_libraries(part2_bin part2_lib)

# Tests
add_executable(part2_test ${PART2_TEST_HEADERS} ${PART2_TEST_SOURCES})
target_link_libraries(part2_test part2_lib)
