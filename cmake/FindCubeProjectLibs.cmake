if (NOT USE_STM_DRIVER)
	if (CUBE_PROJECT)
		list(APPEND USER_DEFINES -DUSE_HAL_DRIVER)
	elseif (CUBE_PROJECT_LL)
		list(APPEND USER_DEFINES -DUSE_LL_DRIVER -DUSE_FULL_LL_DRIVER)
	else()
        # list(APPEND USER_DEFINES -DUSE_STDPERIPH_DRIVER)
        message(FATAL_ERROR "CUBE_PROJECT not defined")
	endif()
else()
	list(APPEND USER_DEFINES -D${USE_STM_DRIVER})
endif()

# Find Include Dirs
if (NOT INCLUDE_DIRS)
    file(GLOB_RECURSE MIDDLEWARE_INCLUDES LIST_DIRECTORIES true "Middlewares/*/Inc")
    file(GLOB_RECURSE DRIVERS_INCLUDES LIST_DIRECTORIES true "Drivers/*/Inc")
    file(GLOB_RECURSE CMSIS_INCLUDES LIST_DIRECTORIES true "CMSIS/*/Inc")
    # file(GLOB_RECURSE DRIVER_INCLUDES LIST_DIRECTORIES true "./Drivers/*/Inc")
    set(INCLUDE_DIRS ${ADD_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/Core/Inc/
        ${MIDDLEWARE_INCLUDES}
        ${DRIVERS_INCLUDES}
        ${CMSIS_INCLUDES})
endif()

# Find Sources
if (NOT SOURCE_FILES)
    file(GLOB_RECURSE MIDDLEWARE_SOURCES "Middlewares/*.c")
    file(GLOB_RECURSE SOURCE_FILES "Core/**/*.cpp" "Core/**/*.c" "Drivers/${STM32_FAMILY}xx_HAL_Driver/Src/*.c")
    list(APPEND SOURCE_FILES ${MIDDLEWARE_SOURCES} ${ADD_SOURCE_FILES})
endif()

# Find Link Libraries
if (NOT LINK_LIBRARIES)
    file(GLOB LINK_LIBRARIES "Src/*.${STATIC_LIB_EXT}")
endif()
list(APPEND LINK_LIBRARIES ${ADD_LINK_LIBRARIES})
