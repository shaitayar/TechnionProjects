# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\TechnionProjects\TechnionProjects\mtm\ex1\part1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\TechnionProjects\TechnionProjects\mtm\ex1\part1\build

# Include any dependencies generated for this target.
include CMakeFiles/AmountSet.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/AmountSet.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/AmountSet.dir/flags.make

CMakeFiles/AmountSet.dir/Test.c.obj: CMakeFiles/AmountSet.dir/flags.make
CMakeFiles/AmountSet.dir/Test.c.obj: ../Test.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\TechnionProjects\TechnionProjects\mtm\ex1\part1\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/AmountSet.dir/Test.c.obj"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\AmountSet.dir\Test.c.obj   -c C:\TechnionProjects\TechnionProjects\mtm\ex1\part1\Test.c

CMakeFiles/AmountSet.dir/Test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/AmountSet.dir/Test.c.i"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\TechnionProjects\TechnionProjects\mtm\ex1\part1\Test.c > CMakeFiles\AmountSet.dir\Test.c.i

CMakeFiles/AmountSet.dir/Test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/AmountSet.dir/Test.c.s"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\TechnionProjects\TechnionProjects\mtm\ex1\part1\Test.c -o CMakeFiles\AmountSet.dir\Test.c.s

CMakeFiles/AmountSet.dir/amount_set_str.c.obj: CMakeFiles/AmountSet.dir/flags.make
CMakeFiles/AmountSet.dir/amount_set_str.c.obj: ../amount_set_str.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\TechnionProjects\TechnionProjects\mtm\ex1\part1\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/AmountSet.dir/amount_set_str.c.obj"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\AmountSet.dir\amount_set_str.c.obj   -c C:\TechnionProjects\TechnionProjects\mtm\ex1\part1\amount_set_str.c

CMakeFiles/AmountSet.dir/amount_set_str.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/AmountSet.dir/amount_set_str.c.i"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\TechnionProjects\TechnionProjects\mtm\ex1\part1\amount_set_str.c > CMakeFiles\AmountSet.dir\amount_set_str.c.i

CMakeFiles/AmountSet.dir/amount_set_str.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/AmountSet.dir/amount_set_str.c.s"
	C:\TDM-GCC-64\bin\x86_64-w64-mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\TechnionProjects\TechnionProjects\mtm\ex1\part1\amount_set_str.c -o CMakeFiles\AmountSet.dir\amount_set_str.c.s

# Object files for target AmountSet
AmountSet_OBJECTS = \
"CMakeFiles/AmountSet.dir/Test.c.obj" \
"CMakeFiles/AmountSet.dir/amount_set_str.c.obj"

# External object files for target AmountSet
AmountSet_EXTERNAL_OBJECTS =

AmountSet.exe: CMakeFiles/AmountSet.dir/Test.c.obj
AmountSet.exe: CMakeFiles/AmountSet.dir/amount_set_str.c.obj
AmountSet.exe: CMakeFiles/AmountSet.dir/build.make
AmountSet.exe: CMakeFiles/AmountSet.dir/linklibs.rsp
AmountSet.exe: CMakeFiles/AmountSet.dir/objects1.rsp
AmountSet.exe: CMakeFiles/AmountSet.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\TechnionProjects\TechnionProjects\mtm\ex1\part1\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable AmountSet.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\AmountSet.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/AmountSet.dir/build: AmountSet.exe

.PHONY : CMakeFiles/AmountSet.dir/build

CMakeFiles/AmountSet.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\AmountSet.dir\cmake_clean.cmake
.PHONY : CMakeFiles/AmountSet.dir/clean

CMakeFiles/AmountSet.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\TechnionProjects\TechnionProjects\mtm\ex1\part1 C:\TechnionProjects\TechnionProjects\mtm\ex1\part1 C:\TechnionProjects\TechnionProjects\mtm\ex1\part1\build C:\TechnionProjects\TechnionProjects\mtm\ex1\part1\build C:\TechnionProjects\TechnionProjects\mtm\ex1\part1\build\CMakeFiles\AmountSet.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/AmountSet.dir/depend

