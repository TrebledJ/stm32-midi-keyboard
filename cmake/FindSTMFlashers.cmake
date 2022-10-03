# Look for CLI of STM32CubeProgrammer, TODO: support linux here
if(APPLE)
	find_program(STM32CubeProg "STM32_Programmer_CLI"
		"/Applications/STMicroelectronics/STM32Cube/STM32CubeProgrammer/STM32CubeProgrammer.app/Contents/MacOs/bin")
	# set(prog "/Applications/STMicroelectronics/STM32Cube/STM32CubeProgrammer/STM32CubeProgrammer.app/Contents/MacOs/bin/STM32_Programmer_CLI")
else()
	find_program(STM32CubeProg "STM32_Programmer_CLI"
		"C:/Program Files/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin"
		"C:/Program Files/STMicroelectronics/STM32CubeProgrammer/bin")
	# set(prog "STM32_Programmer_CLI")
endif()
# Find CLI of ST-LINK Utility
find_program(STLINKUTIL "st-link_cli")
# Look for OpenOCD, this is the most important one
find_program(OPENOCD_EXE "openocd")
