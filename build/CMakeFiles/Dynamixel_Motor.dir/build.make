# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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

#Suppress display of executed commands.
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
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jason-nano/I_want_to_fly-vel_plan_humanoid_new/Motor_Driver/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jason-nano/I_want_to_fly-vel_plan_humanoid_new/Motor_Driver/src/build

# Include any dependencies generated for this target.
include CMakeFiles/Dynamixel_Motor.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Dynamixel_Motor.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Dynamixel_Motor.dir/flags.make

CMakeFiles/Dynamixel_Motor.dir/example.cpp.o: CMakeFiles/Dynamixel_Motor.dir/flags.make
CMakeFiles/Dynamixel_Motor.dir/example.cpp.o: ../example.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jason-nano/I_want_to_fly-vel_plan_humanoid_new/Motor_Driver/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Dynamixel_Motor.dir/example.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Dynamixel_Motor.dir/example.cpp.o -c /home/jason-nano/I_want_to_fly-vel_plan_humanoid_new/Motor_Driver/src/example.cpp

CMakeFiles/Dynamixel_Motor.dir/example.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Dynamixel_Motor.dir/example.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jason-nano/I_want_to_fly-vel_plan_humanoid_new/Motor_Driver/src/example.cpp > CMakeFiles/Dynamixel_Motor.dir/example.cpp.i

CMakeFiles/Dynamixel_Motor.dir/example.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Dynamixel_Motor.dir/example.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jason-nano/I_want_to_fly-vel_plan_humanoid_new/Motor_Driver/src/example.cpp -o CMakeFiles/Dynamixel_Motor.dir/example.cpp.s

# Object files for target Dynamixel_Motor
Dynamixel_Motor_OBJECTS = \
"CMakeFiles/Dynamixel_Motor.dir/example.cpp.o"

# External object files for target Dynamixel_Motor
Dynamixel_Motor_EXTERNAL_OBJECTS =

Dynamixel_Motor: CMakeFiles/Dynamixel_Motor.dir/example.cpp.o
Dynamixel_Motor: CMakeFiles/Dynamixel_Motor.dir/build.make
Dynamixel_Motor: MotorUnion/libMotorUnion.so
Dynamixel_Motor: MotorUnion/Motor/libMotor.so
Dynamixel_Motor: MotorUnion/Motor/dynamixel/libDynamixelSdk.so
Dynamixel_Motor: MotorUnion/Motor/motor/libmotor.so
Dynamixel_Motor: CMakeFiles/Dynamixel_Motor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jason-nano/I_want_to_fly-vel_plan_humanoid_new/Motor_Driver/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Dynamixel_Motor"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Dynamixel_Motor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Dynamixel_Motor.dir/build: Dynamixel_Motor

.PHONY : CMakeFiles/Dynamixel_Motor.dir/build

CMakeFiles/Dynamixel_Motor.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Dynamixel_Motor.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Dynamixel_Motor.dir/clean

CMakeFiles/Dynamixel_Motor.dir/depend:
	cd /home/jason-nano/I_want_to_fly-vel_plan_humanoid_new/Motor_Driver/src/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jason-nano/I_want_to_fly-vel_plan_humanoid_new/Motor_Driver/src /home/jason-nano/I_want_to_fly-vel_plan_humanoid_new/Motor_Driver/src /home/jason-nano/I_want_to_fly-vel_plan_humanoid_new/Motor_Driver/src/build /home/jason-nano/I_want_to_fly-vel_plan_humanoid_new/Motor_Driver/src/build /home/jason-nano/I_want_to_fly-vel_plan_humanoid_new/Motor_Driver/src/build/CMakeFiles/Dynamixel_Motor.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Dynamixel_Motor.dir/depend

