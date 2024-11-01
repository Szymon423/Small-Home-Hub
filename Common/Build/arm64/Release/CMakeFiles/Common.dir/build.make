# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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
CMAKE_SOURCE_DIR = /Server-Cabinet-Controller/Common

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Server-Cabinet-Controller/Common/Build/arm64/Release

# Include any dependencies generated for this target.
include CMakeFiles/Common.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Common.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Common.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Common.dir/flags.make

CMakeFiles/Common.dir/src/communication.cpp.o: CMakeFiles/Common.dir/flags.make
CMakeFiles/Common.dir/src/communication.cpp.o: /Server-Cabinet-Controller/Common/src/communication.cpp
CMakeFiles/Common.dir/src/communication.cpp.o: CMakeFiles/Common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Server-Cabinet-Controller/Common/Build/arm64/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Common.dir/src/communication.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Common.dir/src/communication.cpp.o -MF CMakeFiles/Common.dir/src/communication.cpp.o.d -o CMakeFiles/Common.dir/src/communication.cpp.o -c /Server-Cabinet-Controller/Common/src/communication.cpp

CMakeFiles/Common.dir/src/communication.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Common.dir/src/communication.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Server-Cabinet-Controller/Common/src/communication.cpp > CMakeFiles/Common.dir/src/communication.cpp.i

CMakeFiles/Common.dir/src/communication.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Common.dir/src/communication.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Server-Cabinet-Controller/Common/src/communication.cpp -o CMakeFiles/Common.dir/src/communication.cpp.s

CMakeFiles/Common.dir/src/configuration.cpp.o: CMakeFiles/Common.dir/flags.make
CMakeFiles/Common.dir/src/configuration.cpp.o: /Server-Cabinet-Controller/Common/src/configuration.cpp
CMakeFiles/Common.dir/src/configuration.cpp.o: CMakeFiles/Common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Server-Cabinet-Controller/Common/Build/arm64/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Common.dir/src/configuration.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Common.dir/src/configuration.cpp.o -MF CMakeFiles/Common.dir/src/configuration.cpp.o.d -o CMakeFiles/Common.dir/src/configuration.cpp.o -c /Server-Cabinet-Controller/Common/src/configuration.cpp

CMakeFiles/Common.dir/src/configuration.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Common.dir/src/configuration.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Server-Cabinet-Controller/Common/src/configuration.cpp > CMakeFiles/Common.dir/src/configuration.cpp.i

CMakeFiles/Common.dir/src/configuration.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Common.dir/src/configuration.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Server-Cabinet-Controller/Common/src/configuration.cpp -o CMakeFiles/Common.dir/src/configuration.cpp.s

CMakeFiles/Common.dir/src/database.cpp.o: CMakeFiles/Common.dir/flags.make
CMakeFiles/Common.dir/src/database.cpp.o: /Server-Cabinet-Controller/Common/src/database.cpp
CMakeFiles/Common.dir/src/database.cpp.o: CMakeFiles/Common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Server-Cabinet-Controller/Common/Build/arm64/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Common.dir/src/database.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Common.dir/src/database.cpp.o -MF CMakeFiles/Common.dir/src/database.cpp.o.d -o CMakeFiles/Common.dir/src/database.cpp.o -c /Server-Cabinet-Controller/Common/src/database.cpp

CMakeFiles/Common.dir/src/database.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Common.dir/src/database.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Server-Cabinet-Controller/Common/src/database.cpp > CMakeFiles/Common.dir/src/database.cpp.i

CMakeFiles/Common.dir/src/database.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Common.dir/src/database.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Server-Cabinet-Controller/Common/src/database.cpp -o CMakeFiles/Common.dir/src/database.cpp.s

CMakeFiles/Common.dir/src/events.cpp.o: CMakeFiles/Common.dir/flags.make
CMakeFiles/Common.dir/src/events.cpp.o: /Server-Cabinet-Controller/Common/src/events.cpp
CMakeFiles/Common.dir/src/events.cpp.o: CMakeFiles/Common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Server-Cabinet-Controller/Common/Build/arm64/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Common.dir/src/events.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Common.dir/src/events.cpp.o -MF CMakeFiles/Common.dir/src/events.cpp.o.d -o CMakeFiles/Common.dir/src/events.cpp.o -c /Server-Cabinet-Controller/Common/src/events.cpp

CMakeFiles/Common.dir/src/events.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Common.dir/src/events.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Server-Cabinet-Controller/Common/src/events.cpp > CMakeFiles/Common.dir/src/events.cpp.i

CMakeFiles/Common.dir/src/events.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Common.dir/src/events.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Server-Cabinet-Controller/Common/src/events.cpp -o CMakeFiles/Common.dir/src/events.cpp.s

CMakeFiles/Common.dir/src/groups.cpp.o: CMakeFiles/Common.dir/flags.make
CMakeFiles/Common.dir/src/groups.cpp.o: /Server-Cabinet-Controller/Common/src/groups.cpp
CMakeFiles/Common.dir/src/groups.cpp.o: CMakeFiles/Common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Server-Cabinet-Controller/Common/Build/arm64/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Common.dir/src/groups.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Common.dir/src/groups.cpp.o -MF CMakeFiles/Common.dir/src/groups.cpp.o.d -o CMakeFiles/Common.dir/src/groups.cpp.o -c /Server-Cabinet-Controller/Common/src/groups.cpp

CMakeFiles/Common.dir/src/groups.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Common.dir/src/groups.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Server-Cabinet-Controller/Common/src/groups.cpp > CMakeFiles/Common.dir/src/groups.cpp.i

CMakeFiles/Common.dir/src/groups.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Common.dir/src/groups.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Server-Cabinet-Controller/Common/src/groups.cpp -o CMakeFiles/Common.dir/src/groups.cpp.s

CMakeFiles/Common.dir/src/hardware.cpp.o: CMakeFiles/Common.dir/flags.make
CMakeFiles/Common.dir/src/hardware.cpp.o: /Server-Cabinet-Controller/Common/src/hardware.cpp
CMakeFiles/Common.dir/src/hardware.cpp.o: CMakeFiles/Common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Server-Cabinet-Controller/Common/Build/arm64/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Common.dir/src/hardware.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Common.dir/src/hardware.cpp.o -MF CMakeFiles/Common.dir/src/hardware.cpp.o.d -o CMakeFiles/Common.dir/src/hardware.cpp.o -c /Server-Cabinet-Controller/Common/src/hardware.cpp

CMakeFiles/Common.dir/src/hardware.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Common.dir/src/hardware.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Server-Cabinet-Controller/Common/src/hardware.cpp > CMakeFiles/Common.dir/src/hardware.cpp.i

CMakeFiles/Common.dir/src/hardware.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Common.dir/src/hardware.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Server-Cabinet-Controller/Common/src/hardware.cpp -o CMakeFiles/Common.dir/src/hardware.cpp.s

CMakeFiles/Common.dir/src/log.cpp.o: CMakeFiles/Common.dir/flags.make
CMakeFiles/Common.dir/src/log.cpp.o: /Server-Cabinet-Controller/Common/src/log.cpp
CMakeFiles/Common.dir/src/log.cpp.o: CMakeFiles/Common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Server-Cabinet-Controller/Common/Build/arm64/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Common.dir/src/log.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Common.dir/src/log.cpp.o -MF CMakeFiles/Common.dir/src/log.cpp.o.d -o CMakeFiles/Common.dir/src/log.cpp.o -c /Server-Cabinet-Controller/Common/src/log.cpp

CMakeFiles/Common.dir/src/log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Common.dir/src/log.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Server-Cabinet-Controller/Common/src/log.cpp > CMakeFiles/Common.dir/src/log.cpp.i

CMakeFiles/Common.dir/src/log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Common.dir/src/log.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Server-Cabinet-Controller/Common/src/log.cpp -o CMakeFiles/Common.dir/src/log.cpp.s

CMakeFiles/Common.dir/src/modbus.cpp.o: CMakeFiles/Common.dir/flags.make
CMakeFiles/Common.dir/src/modbus.cpp.o: /Server-Cabinet-Controller/Common/src/modbus.cpp
CMakeFiles/Common.dir/src/modbus.cpp.o: CMakeFiles/Common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Server-Cabinet-Controller/Common/Build/arm64/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/Common.dir/src/modbus.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Common.dir/src/modbus.cpp.o -MF CMakeFiles/Common.dir/src/modbus.cpp.o.d -o CMakeFiles/Common.dir/src/modbus.cpp.o -c /Server-Cabinet-Controller/Common/src/modbus.cpp

CMakeFiles/Common.dir/src/modbus.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Common.dir/src/modbus.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Server-Cabinet-Controller/Common/src/modbus.cpp > CMakeFiles/Common.dir/src/modbus.cpp.i

CMakeFiles/Common.dir/src/modbus.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Common.dir/src/modbus.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Server-Cabinet-Controller/Common/src/modbus.cpp -o CMakeFiles/Common.dir/src/modbus.cpp.s

CMakeFiles/Common.dir/src/runtime.cpp.o: CMakeFiles/Common.dir/flags.make
CMakeFiles/Common.dir/src/runtime.cpp.o: /Server-Cabinet-Controller/Common/src/runtime.cpp
CMakeFiles/Common.dir/src/runtime.cpp.o: CMakeFiles/Common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Server-Cabinet-Controller/Common/Build/arm64/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/Common.dir/src/runtime.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Common.dir/src/runtime.cpp.o -MF CMakeFiles/Common.dir/src/runtime.cpp.o.d -o CMakeFiles/Common.dir/src/runtime.cpp.o -c /Server-Cabinet-Controller/Common/src/runtime.cpp

CMakeFiles/Common.dir/src/runtime.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Common.dir/src/runtime.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Server-Cabinet-Controller/Common/src/runtime.cpp > CMakeFiles/Common.dir/src/runtime.cpp.i

CMakeFiles/Common.dir/src/runtime.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Common.dir/src/runtime.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Server-Cabinet-Controller/Common/src/runtime.cpp -o CMakeFiles/Common.dir/src/runtime.cpp.s

CMakeFiles/Common.dir/src/shared_memory.cpp.o: CMakeFiles/Common.dir/flags.make
CMakeFiles/Common.dir/src/shared_memory.cpp.o: /Server-Cabinet-Controller/Common/src/shared_memory.cpp
CMakeFiles/Common.dir/src/shared_memory.cpp.o: CMakeFiles/Common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Server-Cabinet-Controller/Common/Build/arm64/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/Common.dir/src/shared_memory.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -fPIC -MD -MT CMakeFiles/Common.dir/src/shared_memory.cpp.o -MF CMakeFiles/Common.dir/src/shared_memory.cpp.o.d -o CMakeFiles/Common.dir/src/shared_memory.cpp.o -c /Server-Cabinet-Controller/Common/src/shared_memory.cpp

CMakeFiles/Common.dir/src/shared_memory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Common.dir/src/shared_memory.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -fPIC -E /Server-Cabinet-Controller/Common/src/shared_memory.cpp > CMakeFiles/Common.dir/src/shared_memory.cpp.i

CMakeFiles/Common.dir/src/shared_memory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Common.dir/src/shared_memory.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -fPIC -S /Server-Cabinet-Controller/Common/src/shared_memory.cpp -o CMakeFiles/Common.dir/src/shared_memory.cpp.s

CMakeFiles/Common.dir/src/signals.cpp.o: CMakeFiles/Common.dir/flags.make
CMakeFiles/Common.dir/src/signals.cpp.o: /Server-Cabinet-Controller/Common/src/signals.cpp
CMakeFiles/Common.dir/src/signals.cpp.o: CMakeFiles/Common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Server-Cabinet-Controller/Common/Build/arm64/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/Common.dir/src/signals.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Common.dir/src/signals.cpp.o -MF CMakeFiles/Common.dir/src/signals.cpp.o.d -o CMakeFiles/Common.dir/src/signals.cpp.o -c /Server-Cabinet-Controller/Common/src/signals.cpp

CMakeFiles/Common.dir/src/signals.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Common.dir/src/signals.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Server-Cabinet-Controller/Common/src/signals.cpp > CMakeFiles/Common.dir/src/signals.cpp.i

CMakeFiles/Common.dir/src/signals.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Common.dir/src/signals.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Server-Cabinet-Controller/Common/src/signals.cpp -o CMakeFiles/Common.dir/src/signals.cpp.s

CMakeFiles/Common.dir/src/snmp.cpp.o: CMakeFiles/Common.dir/flags.make
CMakeFiles/Common.dir/src/snmp.cpp.o: /Server-Cabinet-Controller/Common/src/snmp.cpp
CMakeFiles/Common.dir/src/snmp.cpp.o: CMakeFiles/Common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Server-Cabinet-Controller/Common/Build/arm64/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/Common.dir/src/snmp.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Common.dir/src/snmp.cpp.o -MF CMakeFiles/Common.dir/src/snmp.cpp.o.d -o CMakeFiles/Common.dir/src/snmp.cpp.o -c /Server-Cabinet-Controller/Common/src/snmp.cpp

CMakeFiles/Common.dir/src/snmp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Common.dir/src/snmp.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Server-Cabinet-Controller/Common/src/snmp.cpp > CMakeFiles/Common.dir/src/snmp.cpp.i

CMakeFiles/Common.dir/src/snmp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Common.dir/src/snmp.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Server-Cabinet-Controller/Common/src/snmp.cpp -o CMakeFiles/Common.dir/src/snmp.cpp.s

CMakeFiles/Common.dir/src/users.cpp.o: CMakeFiles/Common.dir/flags.make
CMakeFiles/Common.dir/src/users.cpp.o: /Server-Cabinet-Controller/Common/src/users.cpp
CMakeFiles/Common.dir/src/users.cpp.o: CMakeFiles/Common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Server-Cabinet-Controller/Common/Build/arm64/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/Common.dir/src/users.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Common.dir/src/users.cpp.o -MF CMakeFiles/Common.dir/src/users.cpp.o.d -o CMakeFiles/Common.dir/src/users.cpp.o -c /Server-Cabinet-Controller/Common/src/users.cpp

CMakeFiles/Common.dir/src/users.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Common.dir/src/users.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Server-Cabinet-Controller/Common/src/users.cpp > CMakeFiles/Common.dir/src/users.cpp.i

CMakeFiles/Common.dir/src/users.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Common.dir/src/users.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Server-Cabinet-Controller/Common/src/users.cpp -o CMakeFiles/Common.dir/src/users.cpp.s

CMakeFiles/Common.dir/src/utilities.cpp.o: CMakeFiles/Common.dir/flags.make
CMakeFiles/Common.dir/src/utilities.cpp.o: /Server-Cabinet-Controller/Common/src/utilities.cpp
CMakeFiles/Common.dir/src/utilities.cpp.o: CMakeFiles/Common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Server-Cabinet-Controller/Common/Build/arm64/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/Common.dir/src/utilities.cpp.o"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Common.dir/src/utilities.cpp.o -MF CMakeFiles/Common.dir/src/utilities.cpp.o.d -o CMakeFiles/Common.dir/src/utilities.cpp.o -c /Server-Cabinet-Controller/Common/src/utilities.cpp

CMakeFiles/Common.dir/src/utilities.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Common.dir/src/utilities.cpp.i"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Server-Cabinet-Controller/Common/src/utilities.cpp > CMakeFiles/Common.dir/src/utilities.cpp.i

CMakeFiles/Common.dir/src/utilities.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Common.dir/src/utilities.cpp.s"
	/usr/bin/aarch64-linux-gnu-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Server-Cabinet-Controller/Common/src/utilities.cpp -o CMakeFiles/Common.dir/src/utilities.cpp.s

# Object files for target Common
Common_OBJECTS = \
"CMakeFiles/Common.dir/src/communication.cpp.o" \
"CMakeFiles/Common.dir/src/configuration.cpp.o" \
"CMakeFiles/Common.dir/src/database.cpp.o" \
"CMakeFiles/Common.dir/src/events.cpp.o" \
"CMakeFiles/Common.dir/src/groups.cpp.o" \
"CMakeFiles/Common.dir/src/hardware.cpp.o" \
"CMakeFiles/Common.dir/src/log.cpp.o" \
"CMakeFiles/Common.dir/src/modbus.cpp.o" \
"CMakeFiles/Common.dir/src/runtime.cpp.o" \
"CMakeFiles/Common.dir/src/shared_memory.cpp.o" \
"CMakeFiles/Common.dir/src/signals.cpp.o" \
"CMakeFiles/Common.dir/src/snmp.cpp.o" \
"CMakeFiles/Common.dir/src/users.cpp.o" \
"CMakeFiles/Common.dir/src/utilities.cpp.o"

# External object files for target Common
Common_EXTERNAL_OBJECTS =

libCommon.a: CMakeFiles/Common.dir/src/communication.cpp.o
libCommon.a: CMakeFiles/Common.dir/src/configuration.cpp.o
libCommon.a: CMakeFiles/Common.dir/src/database.cpp.o
libCommon.a: CMakeFiles/Common.dir/src/events.cpp.o
libCommon.a: CMakeFiles/Common.dir/src/groups.cpp.o
libCommon.a: CMakeFiles/Common.dir/src/hardware.cpp.o
libCommon.a: CMakeFiles/Common.dir/src/log.cpp.o
libCommon.a: CMakeFiles/Common.dir/src/modbus.cpp.o
libCommon.a: CMakeFiles/Common.dir/src/runtime.cpp.o
libCommon.a: CMakeFiles/Common.dir/src/shared_memory.cpp.o
libCommon.a: CMakeFiles/Common.dir/src/signals.cpp.o
libCommon.a: CMakeFiles/Common.dir/src/snmp.cpp.o
libCommon.a: CMakeFiles/Common.dir/src/users.cpp.o
libCommon.a: CMakeFiles/Common.dir/src/utilities.cpp.o
libCommon.a: CMakeFiles/Common.dir/build.make
libCommon.a: CMakeFiles/Common.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Server-Cabinet-Controller/Common/Build/arm64/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Linking CXX static library libCommon.a"
	$(CMAKE_COMMAND) -P CMakeFiles/Common.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Common.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Common.dir/build: libCommon.a
.PHONY : CMakeFiles/Common.dir/build

CMakeFiles/Common.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Common.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Common.dir/clean

CMakeFiles/Common.dir/depend:
	cd /Server-Cabinet-Controller/Common/Build/arm64/Release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Server-Cabinet-Controller/Common /Server-Cabinet-Controller/Common /Server-Cabinet-Controller/Common/Build/arm64/Release /Server-Cabinet-Controller/Common/Build/arm64/Release /Server-Cabinet-Controller/Common/Build/arm64/Release/CMakeFiles/Common.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Common.dir/depend
