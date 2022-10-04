# CMake Project File
#
# If you're changing CMake stuff and it doesnt seem to change, delete the CMakeCache.txt file and retry
#
# For Windows, CMake tries to use Visual Studio if available, this fucks up the custom toolchain setting in here
# Always manually override to Ninja or Make
#
# For all platforms Ninja is much faster than Make so use it

cmake_minimum_required(VERSION 3.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(utils)

# Sanity Check: The STM32 model number needs to be defined
if(NOT DEFINED STM32FXXX OR STM32FXXX STREQUAL "")
	message(FATAL_ERROR "Define which STM32 MCU you are using in project.cmake (set STM32FXXX)")
endif()

# Default to Release Mode, 
# CMake doesn't recommend this, but Debug mode doesn't work on the robot usually 
# So make it idiot proof
if (NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
	message("CMAKE_BUILD_TYPE defaulting to Release")
	set(CMAKE_BUILD_TYPE "Release")
endif()

# I dont even know if this is used but here is the crystal oscillator frequency
if (NOT DEFINED XTAL)
	set(XTAL 8000000)
endif()

include(parse_stm_model)

find_package(OpenocdCfg)

set(STATIC_LIB_EXT "a")

##############################################################################################################################################################################

find_package(MCUStartupFile)
find_package(LinkerScript)

# Find sources, includes, and link libraries
find_package(CubeProjectLibs)

##############################################################################################################################################################################

# Print all the information for debugging
message("")
message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
if (MCU_STARTUP_FILE)
	file(RELATIVE_PATH PRINT_MCU_STARTUP_FILE ${CMAKE_CURRENT_SOURCE_DIR} ${MCU_STARTUP_FILE})
	message("MCU_STARTUP_FILE: ${PRINT_MCU_STARTUP_FILE}")
else()
	message(FATAL_ERROR "Could not find a startup file")
endif()

if (LINKER_SCRIPT)
	file(RELATIVE_PATH PRINT_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR} ${LINKER_SCRIPT})
	message("LINKER_SCRIPT: ${PRINT_LINKER_SCRIPT}")
else()
	message(FATAL_ERROR "Could not find a linker script")
endif()

join("${USER_DEFINES}" " " PRINT_USER_DEFINES)
message("USER_DEFINES = ${PRINT_USER_DEFINES}")

message("SOURCE_FILES = ")
pprint_files(SOURCE_FILES)

message("INCLUDE_DIRS = ")
pprint_files(INCLUDE_DIRS)

if(LINK_LIBRARIES)
	message("LINK_LIBRARIES (${STATIC_LIB_EXT}) =")
	pprint_files(LINK_LIBRARIES)
else()
	message("LINK_LIBRARIES (${STATIC_LIB_EXT}) = (none)")
endif()
message("")