# Find the MCU startup files. The life before main() is a strange and wonderful time.
# STMicro provides handwritten assembly that defines what happens at this stage of life.
# We must find it in their Nonsensical-ass naming convention, how? try absolutely everything in rough order of more common to rare
# This still usually fails when using STM32CubeMX projects because there is absolutely no rhyme or reason to STMicros ways
if (NOT MCU_STARTUP_FILE)
	# usually the startup file is here but who knows!?
	set(MCU_STARTUP_SEARCH_DIRS
		"." "./MDK-ARM" "./Core/Startup")
	message("Looking for Startup Assembly in ${MCU_STARTUP_SEARCH_DIRS}")
	# search for startup_ followed by some variation of the stm32 model no.
	find_file(MCU_STARTUP_FILE "startup_${stm32fxxx}.s" ${MCU_STARTUP_SEARCH_DIRS}
		NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_FIND_ROOT_PATH)
	find_file(MCU_STARTUP_FILE "startup_${stm32_series_b}.s" ${MCU_STARTUP_SEARCH_DIRS}
		NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_FIND_ROOT_PATH)
	find_file(MCU_STARTUP_FILE "startup_${stm32_series_a}.s" ${MCU_STARTUP_SEARCH_DIRS}
		NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_FIND_ROOT_PATH)
	find_file(MCU_STARTUP_FILE "startup_${stm32_series}xx.s" ${MCU_STARTUP_SEARCH_DIRS}
		NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_FIND_ROOT_PATH)
	find_file(MCU_STARTUP_FILE "startup_${stm32_family}xx.s" ${MCU_STARTUP_SEARCH_DIRS}
		NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_FIND_ROOT_PATH)
	find_file(MCU_STARTUP_FILE "startup_${stm32fxxx}tx.s" ${MCU_STARTUP_SEARCH_DIRS}
		NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_FIND_ROOT_PATH)
	if (CUBE_PROJECT OR CUBE_PROJECT_LL)
		# OR Just literally anything starting with startup_ and ending with .s
		file(GLOB_RECURSE MCU_STARTUP_FILE "startup_*.s" NO_CMAKE_FIND_ROOT_PATH)
	endif()
endif()
