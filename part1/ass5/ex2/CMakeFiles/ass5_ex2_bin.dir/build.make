# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/svilen/TUM/Praktikum/part1/ass5/ex2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/svilen/TUM/Praktikum/part1/ass5/ex2

# Include any dependencies generated for this target.
include CMakeFiles/ass5_ex2_bin.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ass5_ex2_bin.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ass5_ex2_bin.dir/flags.make

qrc_imageFiles.cpp: images/pen.png
qrc_imageFiles.cpp: images/cursor-arrow.png
qrc_imageFiles.cpp: images/color-picker-icon.png
qrc_imageFiles.cpp: images/pick-color-icon.png
qrc_imageFiles.cpp: imageFiles.qrc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/svilen/TUM/Praktikum/part1/ass5/ex2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating qrc_imageFiles.cpp"
	/usr/lib/x86_64-linux-gnu/qt5/bin/rcc --name imageFiles --output /home/svilen/TUM/Praktikum/part1/ass5/ex2/qrc_imageFiles.cpp /home/svilen/TUM/Praktikum/part1/ass5/ex2/imageFiles.qrc

CMakeFiles/ass5_ex2_bin.dir/src/DrawingWidget.cpp.o: CMakeFiles/ass5_ex2_bin.dir/flags.make
CMakeFiles/ass5_ex2_bin.dir/src/DrawingWidget.cpp.o: src/DrawingWidget.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/svilen/TUM/Praktikum/part1/ass5/ex2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ass5_ex2_bin.dir/src/DrawingWidget.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ass5_ex2_bin.dir/src/DrawingWidget.cpp.o -c /home/svilen/TUM/Praktikum/part1/ass5/ex2/src/DrawingWidget.cpp

CMakeFiles/ass5_ex2_bin.dir/src/DrawingWidget.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ass5_ex2_bin.dir/src/DrawingWidget.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/svilen/TUM/Praktikum/part1/ass5/ex2/src/DrawingWidget.cpp > CMakeFiles/ass5_ex2_bin.dir/src/DrawingWidget.cpp.i

CMakeFiles/ass5_ex2_bin.dir/src/DrawingWidget.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ass5_ex2_bin.dir/src/DrawingWidget.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/svilen/TUM/Praktikum/part1/ass5/ex2/src/DrawingWidget.cpp -o CMakeFiles/ass5_ex2_bin.dir/src/DrawingWidget.cpp.s

CMakeFiles/ass5_ex2_bin.dir/src/DrawingWidget.cpp.o.requires:

.PHONY : CMakeFiles/ass5_ex2_bin.dir/src/DrawingWidget.cpp.o.requires

CMakeFiles/ass5_ex2_bin.dir/src/DrawingWidget.cpp.o.provides: CMakeFiles/ass5_ex2_bin.dir/src/DrawingWidget.cpp.o.requires
	$(MAKE) -f CMakeFiles/ass5_ex2_bin.dir/build.make CMakeFiles/ass5_ex2_bin.dir/src/DrawingWidget.cpp.o.provides.build
.PHONY : CMakeFiles/ass5_ex2_bin.dir/src/DrawingWidget.cpp.o.provides

CMakeFiles/ass5_ex2_bin.dir/src/DrawingWidget.cpp.o.provides.build: CMakeFiles/ass5_ex2_bin.dir/src/DrawingWidget.cpp.o


CMakeFiles/ass5_ex2_bin.dir/src/MainWindow.cpp.o: CMakeFiles/ass5_ex2_bin.dir/flags.make
CMakeFiles/ass5_ex2_bin.dir/src/MainWindow.cpp.o: src/MainWindow.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/svilen/TUM/Praktikum/part1/ass5/ex2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/ass5_ex2_bin.dir/src/MainWindow.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ass5_ex2_bin.dir/src/MainWindow.cpp.o -c /home/svilen/TUM/Praktikum/part1/ass5/ex2/src/MainWindow.cpp

CMakeFiles/ass5_ex2_bin.dir/src/MainWindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ass5_ex2_bin.dir/src/MainWindow.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/svilen/TUM/Praktikum/part1/ass5/ex2/src/MainWindow.cpp > CMakeFiles/ass5_ex2_bin.dir/src/MainWindow.cpp.i

CMakeFiles/ass5_ex2_bin.dir/src/MainWindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ass5_ex2_bin.dir/src/MainWindow.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/svilen/TUM/Praktikum/part1/ass5/ex2/src/MainWindow.cpp -o CMakeFiles/ass5_ex2_bin.dir/src/MainWindow.cpp.s

CMakeFiles/ass5_ex2_bin.dir/src/MainWindow.cpp.o.requires:

.PHONY : CMakeFiles/ass5_ex2_bin.dir/src/MainWindow.cpp.o.requires

CMakeFiles/ass5_ex2_bin.dir/src/MainWindow.cpp.o.provides: CMakeFiles/ass5_ex2_bin.dir/src/MainWindow.cpp.o.requires
	$(MAKE) -f CMakeFiles/ass5_ex2_bin.dir/build.make CMakeFiles/ass5_ex2_bin.dir/src/MainWindow.cpp.o.provides.build
.PHONY : CMakeFiles/ass5_ex2_bin.dir/src/MainWindow.cpp.o.provides

CMakeFiles/ass5_ex2_bin.dir/src/MainWindow.cpp.o.provides.build: CMakeFiles/ass5_ex2_bin.dir/src/MainWindow.cpp.o


CMakeFiles/ass5_ex2_bin.dir/src/main.cpp.o: CMakeFiles/ass5_ex2_bin.dir/flags.make
CMakeFiles/ass5_ex2_bin.dir/src/main.cpp.o: src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/svilen/TUM/Praktikum/part1/ass5/ex2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/ass5_ex2_bin.dir/src/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ass5_ex2_bin.dir/src/main.cpp.o -c /home/svilen/TUM/Praktikum/part1/ass5/ex2/src/main.cpp

CMakeFiles/ass5_ex2_bin.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ass5_ex2_bin.dir/src/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/svilen/TUM/Praktikum/part1/ass5/ex2/src/main.cpp > CMakeFiles/ass5_ex2_bin.dir/src/main.cpp.i

CMakeFiles/ass5_ex2_bin.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ass5_ex2_bin.dir/src/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/svilen/TUM/Praktikum/part1/ass5/ex2/src/main.cpp -o CMakeFiles/ass5_ex2_bin.dir/src/main.cpp.s

CMakeFiles/ass5_ex2_bin.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/ass5_ex2_bin.dir/src/main.cpp.o.requires

CMakeFiles/ass5_ex2_bin.dir/src/main.cpp.o.provides: CMakeFiles/ass5_ex2_bin.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/ass5_ex2_bin.dir/build.make CMakeFiles/ass5_ex2_bin.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/ass5_ex2_bin.dir/src/main.cpp.o.provides

CMakeFiles/ass5_ex2_bin.dir/src/main.cpp.o.provides.build: CMakeFiles/ass5_ex2_bin.dir/src/main.cpp.o


CMakeFiles/ass5_ex2_bin.dir/qrc_imageFiles.cpp.o: CMakeFiles/ass5_ex2_bin.dir/flags.make
CMakeFiles/ass5_ex2_bin.dir/qrc_imageFiles.cpp.o: qrc_imageFiles.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/svilen/TUM/Praktikum/part1/ass5/ex2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/ass5_ex2_bin.dir/qrc_imageFiles.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ass5_ex2_bin.dir/qrc_imageFiles.cpp.o -c /home/svilen/TUM/Praktikum/part1/ass5/ex2/qrc_imageFiles.cpp

CMakeFiles/ass5_ex2_bin.dir/qrc_imageFiles.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ass5_ex2_bin.dir/qrc_imageFiles.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/svilen/TUM/Praktikum/part1/ass5/ex2/qrc_imageFiles.cpp > CMakeFiles/ass5_ex2_bin.dir/qrc_imageFiles.cpp.i

CMakeFiles/ass5_ex2_bin.dir/qrc_imageFiles.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ass5_ex2_bin.dir/qrc_imageFiles.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/svilen/TUM/Praktikum/part1/ass5/ex2/qrc_imageFiles.cpp -o CMakeFiles/ass5_ex2_bin.dir/qrc_imageFiles.cpp.s

CMakeFiles/ass5_ex2_bin.dir/qrc_imageFiles.cpp.o.requires:

.PHONY : CMakeFiles/ass5_ex2_bin.dir/qrc_imageFiles.cpp.o.requires

CMakeFiles/ass5_ex2_bin.dir/qrc_imageFiles.cpp.o.provides: CMakeFiles/ass5_ex2_bin.dir/qrc_imageFiles.cpp.o.requires
	$(MAKE) -f CMakeFiles/ass5_ex2_bin.dir/build.make CMakeFiles/ass5_ex2_bin.dir/qrc_imageFiles.cpp.o.provides.build
.PHONY : CMakeFiles/ass5_ex2_bin.dir/qrc_imageFiles.cpp.o.provides

CMakeFiles/ass5_ex2_bin.dir/qrc_imageFiles.cpp.o.provides.build: CMakeFiles/ass5_ex2_bin.dir/qrc_imageFiles.cpp.o


CMakeFiles/ass5_ex2_bin.dir/ass5_ex2_bin_automoc.cpp.o: CMakeFiles/ass5_ex2_bin.dir/flags.make
CMakeFiles/ass5_ex2_bin.dir/ass5_ex2_bin_automoc.cpp.o: ass5_ex2_bin_automoc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/svilen/TUM/Praktikum/part1/ass5/ex2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/ass5_ex2_bin.dir/ass5_ex2_bin_automoc.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ass5_ex2_bin.dir/ass5_ex2_bin_automoc.cpp.o -c /home/svilen/TUM/Praktikum/part1/ass5/ex2/ass5_ex2_bin_automoc.cpp

CMakeFiles/ass5_ex2_bin.dir/ass5_ex2_bin_automoc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ass5_ex2_bin.dir/ass5_ex2_bin_automoc.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/svilen/TUM/Praktikum/part1/ass5/ex2/ass5_ex2_bin_automoc.cpp > CMakeFiles/ass5_ex2_bin.dir/ass5_ex2_bin_automoc.cpp.i

CMakeFiles/ass5_ex2_bin.dir/ass5_ex2_bin_automoc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ass5_ex2_bin.dir/ass5_ex2_bin_automoc.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/svilen/TUM/Praktikum/part1/ass5/ex2/ass5_ex2_bin_automoc.cpp -o CMakeFiles/ass5_ex2_bin.dir/ass5_ex2_bin_automoc.cpp.s

CMakeFiles/ass5_ex2_bin.dir/ass5_ex2_bin_automoc.cpp.o.requires:

.PHONY : CMakeFiles/ass5_ex2_bin.dir/ass5_ex2_bin_automoc.cpp.o.requires

CMakeFiles/ass5_ex2_bin.dir/ass5_ex2_bin_automoc.cpp.o.provides: CMakeFiles/ass5_ex2_bin.dir/ass5_ex2_bin_automoc.cpp.o.requires
	$(MAKE) -f CMakeFiles/ass5_ex2_bin.dir/build.make CMakeFiles/ass5_ex2_bin.dir/ass5_ex2_bin_automoc.cpp.o.provides.build
.PHONY : CMakeFiles/ass5_ex2_bin.dir/ass5_ex2_bin_automoc.cpp.o.provides

CMakeFiles/ass5_ex2_bin.dir/ass5_ex2_bin_automoc.cpp.o.provides.build: CMakeFiles/ass5_ex2_bin.dir/ass5_ex2_bin_automoc.cpp.o


# Object files for target ass5_ex2_bin
ass5_ex2_bin_OBJECTS = \
"CMakeFiles/ass5_ex2_bin.dir/src/DrawingWidget.cpp.o" \
"CMakeFiles/ass5_ex2_bin.dir/src/MainWindow.cpp.o" \
"CMakeFiles/ass5_ex2_bin.dir/src/main.cpp.o" \
"CMakeFiles/ass5_ex2_bin.dir/qrc_imageFiles.cpp.o" \
"CMakeFiles/ass5_ex2_bin.dir/ass5_ex2_bin_automoc.cpp.o"

# External object files for target ass5_ex2_bin
ass5_ex2_bin_EXTERNAL_OBJECTS =

ass5_ex2_bin: CMakeFiles/ass5_ex2_bin.dir/src/DrawingWidget.cpp.o
ass5_ex2_bin: CMakeFiles/ass5_ex2_bin.dir/src/MainWindow.cpp.o
ass5_ex2_bin: CMakeFiles/ass5_ex2_bin.dir/src/main.cpp.o
ass5_ex2_bin: CMakeFiles/ass5_ex2_bin.dir/qrc_imageFiles.cpp.o
ass5_ex2_bin: CMakeFiles/ass5_ex2_bin.dir/ass5_ex2_bin_automoc.cpp.o
ass5_ex2_bin: CMakeFiles/ass5_ex2_bin.dir/build.make
ass5_ex2_bin: /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5.5.1
ass5_ex2_bin: /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5.5.1
ass5_ex2_bin: /usr/lib/x86_64-linux-gnu/libQt5Core.so.5.5.1
ass5_ex2_bin: CMakeFiles/ass5_ex2_bin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/svilen/TUM/Praktikum/part1/ass5/ex2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable ass5_ex2_bin"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ass5_ex2_bin.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ass5_ex2_bin.dir/build: ass5_ex2_bin

.PHONY : CMakeFiles/ass5_ex2_bin.dir/build

CMakeFiles/ass5_ex2_bin.dir/requires: CMakeFiles/ass5_ex2_bin.dir/src/DrawingWidget.cpp.o.requires
CMakeFiles/ass5_ex2_bin.dir/requires: CMakeFiles/ass5_ex2_bin.dir/src/MainWindow.cpp.o.requires
CMakeFiles/ass5_ex2_bin.dir/requires: CMakeFiles/ass5_ex2_bin.dir/src/main.cpp.o.requires
CMakeFiles/ass5_ex2_bin.dir/requires: CMakeFiles/ass5_ex2_bin.dir/qrc_imageFiles.cpp.o.requires
CMakeFiles/ass5_ex2_bin.dir/requires: CMakeFiles/ass5_ex2_bin.dir/ass5_ex2_bin_automoc.cpp.o.requires

.PHONY : CMakeFiles/ass5_ex2_bin.dir/requires

CMakeFiles/ass5_ex2_bin.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ass5_ex2_bin.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ass5_ex2_bin.dir/clean

CMakeFiles/ass5_ex2_bin.dir/depend: qrc_imageFiles.cpp
	cd /home/svilen/TUM/Praktikum/part1/ass5/ex2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/svilen/TUM/Praktikum/part1/ass5/ex2 /home/svilen/TUM/Praktikum/part1/ass5/ex2 /home/svilen/TUM/Praktikum/part1/ass5/ex2 /home/svilen/TUM/Praktikum/part1/ass5/ex2 /home/svilen/TUM/Praktikum/part1/ass5/ex2/CMakeFiles/ass5_ex2_bin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ass5_ex2_bin.dir/depend
