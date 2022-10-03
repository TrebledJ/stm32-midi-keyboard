cmake_minimum_required(VERSION 3.0)

# Use the arm-none-eabi-gcc compiler (provide a toolchain file to use something else like clang)
if (NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE cmake/arm-gcc.toolchain)
endif()

##############################################################################################################################################################################

# Set the variable and resource directories for the current project

# Change the Project Name
set(PROJ_NAME "stm32-midi-keyboard")

# Change the MCU model
set(STM32FXXX "STM32F405RG")
# set(USE_GCC 1)                # Use gcc instead of armcc.
set(CUBE_PROJECT 1)           # This is a cube generated project (DCUBE_PROJECT_LL means use LL).
# set(USE_LTO 1)                # Link time optimisation.
# set(USE_OFAST 1)              # Optimise for speed.


# User Configuration 
# - If your project is a special snowflake you can customize some stuff here, most defaults are good tho

# C DEFINES
# set(USER_DEFINES -DSOMETHING)

# Source Files - Any C file anywhere inside the src/ folder will be compiled and linked
# file(GLOB_RECURSE SOURCE_FILES "./src/*.c")
# set(SOURCE_FILES ${SOURCE_FILES})

# Include Directories - Compiler searches here for included header files 
# file(GLOB INCLUDE_DIRS "./src/*/") # Any directory in src
# set(INCLUDE_DIRS ${INCLUDE_DIRS}) # Keil-specific garbage

# Statically Linked Libraries
# file(GLOB LINK_LIBRARIES "./lib/${USE_COMPILER}/*")

# Scatter File - Used by linker to layout memory
# get_filename_component(SCATTER_FILE ./STM32F429_439xx/STM32F429_439xx.sct ABSOLUTE)

# set(MCU_STARTUP_DIR "./Core/Startup") # Will go in to compiler specific directory and then find MCU specific startup file
# set(MCU_STARTUP_FILENAME startup_stm32f405rgtx.s)
set(MCU_STARTUP_FILE 0) # Directly use this as the startup filename
##############################################################################################################################################################################

project (${PROJ_NAME} C CXX ASM)

include(cmake/init.cmake)
include(cmake/exe.cmake)
