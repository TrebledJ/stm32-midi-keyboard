# Find the OpenOCD config in the following order of priority:
if (NOT OPENOCD_CFG)
    # TODO: search a global directory
	# openocd.cfg in this folder or src/ or the build directory
	find_file(OPENOCD_CFG_FILE "openocd.cfg" "." "./src" "${CMAKE_BINARY_DIR}"
		NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH)
	# .cfg named after the stm32_family in the same places
	find_file(OPENOCD_CFG_FILE "${stm32_family}x.cfg" "." "./src" "${CMAKE_BINARY_DIR}"
		NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH)
	if (OPENOCD_CFG_FILE)
		set(OPENOCD_CFG -f ${OPENOCD_CFG_FILE})
		message("Found openocd.cfg: ${OPENOCD_CFG_FILE}")
	else()
		# Use interface and target given by openocd
		set(OPENOCD_CFG -f interface/stlink-v2.cfg -f target/${stm32_family}x.cfg)
	endif()
endif()

