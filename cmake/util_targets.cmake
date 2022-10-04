# Find Flashing Tools
find_package(STMFlashers)

find_file(GDB_DEBUG_FILE "../debug.gdb" "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}"
	NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH)

function(stlinkutil_targets)
	if(STLINKUTIL)
		add_custom_target(flash_stlinkutil
			${STLINKUTIL} -c -P ${PROJECT_NAME}.hex -Rst -Run
			DEPENDS ${PROJECT_NAME}
			COMMENT "Flashing target hardware (by STLink Utility CLI)")

		add_custom_target(erase_stlinkutil
			${STLINKUTIL} -c -ME
			COMMENT "Mass Erase all flash sectors")

		set(FLASH_TARGET flash_stlinkutil PARENT_SCOPE)
		set(ERASE_TARGET erase_stlinkutil PARENT_SCOPE)
	endif()
endfunction()

function(stflash_targets)
	if(STFLASH_EXE)
		add_custom_target(flash_stflash
			${STFLASH_EXE} --reset --format ihex write ${PROJECT_NAME}.hex
			DEPENDS ${PROJECT_NAME}
			COMMENT "Flashing target hardware (by Texane/STlink) (Remember to reset to run the program)")

		add_custom_target(erase_stflash
			${STFLASH_EXE} erase
			COMMENT "Mass Erase all flash sectors")

		find_program(STUTIL "st-util")
		if (STUTIL)
			add_custom_target(debug_stutil
			${STUTIL} -m -p 3333
			DEPENDS ${PROJECT_NAME}
			COMMENT "Running ST-UTIL as a GDB host (port 3333)")
			set(DEBUG_TARGET debug_stutil PARENT_SCOPE)
		endif()

		set(FLASH_TARGET flash_stflash PARENT_SCOPE)
		set(ERASE_TARGET erase_stflash PARENT_SCOPE)
	endif()
endfunction()

function(cubeprog_targets)
	if (STM32CubeProg)
		add_custom_target(flash_cubeprog
			${STM32CubeProg} -c port=SWD -w ${PROJECT_NAME}.elf -g
			DEPENDS ${PROJECT_NAME}
			COMMENT "Flashing target hardware (by STM32CubeProgrammer)")
		add_custom_target(flash_uart
			${STM32CubeProg} -c port=COM5 -w ${PROJECT_NAME}.elf -g
			DEPENDS ${PROJECT_NAME}
			COMMENT "Flashing target hardware (by UART)")
		add_custom_target(flash_usb
			${STM32CubeProg} -c port=usb0 -w ${PROJECT_NAME}.elf -g
			DEPENDS ${PROJECT_NAME}
			COMMENT "Flashing target hardware (by USB DFU)")
		add_custom_target(erase_cubeprog
			${STM32CubeProg} -c port-SWD -e all
			COMMENT "Mass Erase all flash sectors")
		
		add_custom_target(debug_cubeprog
			echo "Debug server unimplemented for Cube Programmer")
		add_custom_target(gdb_cubeprog
			echo "GDB unimplemented for Cube Programmer")

		set(FLASH_TARGET flash_cubeprog PARENT_SCOPE)
		set(ERASE_TARGET erase_cubeprog PARENT_SCOPE)
		# TODO: GDB Debug Server
	endif()
endfunction()

function(openocd_targets)
	if (OPENOCD_EXE)
		add_custom_target(flash_openocd
		  ${OPENOCD_EXE} ${OPENOCD_CFG} -c "transport select hla_swd"
		  -c "init" -c "reset halt"
		  -c "flash write_image erase ${PROJECT_NAME}.elf" -c "reset run" -c "shutdown"
		  DEPENDS ${PROJECT_NAME}
		  COMMENT "Flashing target hardware (by OpenOCD)")

		add_custom_target(debug_openocd
		  ${OPENOCD_EXE} ${OPENOCD_CFG}
		  DEPENDS ${PROJECT_NAME}
		  COMMENT "Running OpenOCD as a GDB host (port 3333)")

		add_custom_target(gdb_openocd
			gdbgui -g arm-none-eabi-gdb ${PROJECT_NAME}.elf --gdb-args="-x ${GDB_DEBUG_FILE}")

		add_custom_target(erase_openocd
		  ${OPENOCD_EXE} ${OPENOCD_CFG} -c "transport select hla_swd"
		  -c "init" -c "reset halt"
		  -c "flash erase_sector 0 0 last" -c "shutdown"
		  DEPENDS ${PROJECT_NAME}
		  COMMENT "Mass Erase all flash sectors")

		set(FLASH_TARGET flash_openocd PARENT_SCOPE)
		set(ERASE_TARGET erase_openocd PARENT_SCOPE)
		set(DEBUG_TARGET debug_openocd PARENT_SCOPE)
		set(GDB_TARGET gdb_openocd PARENT_SCOPE)
	endif()
endfunction()

function(blackmagic_targets)
    if(NOT WINDOWS)
		add_custom_target(flash_blackmagic
		  arm-none-eabi-gdb ${PROJECT_NAME}.elf 
            -ex 'target extended-remote /dev/cu.usbmodemB5D699FD1'
            -ex 'monitor swdp_scan'
            -ex 'att 1'
            -ex 'load'
            -ex 'compare-sections'
            -ex 'set confirm off'
            -ex 'quit'
            DEPENDS ${PROJECT_NAME}
            COMMENT "Flashing target hardware (by BMP)")
    else()
        add_custom_target(flash_blackmagic
		  arm-none-eabi-gdb ${PROJECT_NAME}.elf 
            -ex 'target extended-remote COM#'
            -ex 'att 1'
            -ex 'load'
            -ex 'compare-sections'
            -ex 'set confirm off'
            -ex 'quit'
            DEPENDS ${PROJECT_NAME}
            COMMENT "Flashing target hardware (by BMP)")
    endif()
    set(FLASH_TARGET flash_blackmagic PARENT_SCOPE)
    #set(ERASE_TARGET erase_blackmagic PARENT_SCOPE)
endfunction()

if(NOT DEFINED DEFAULT_PROGRAMMER)
	blackmagic_targets()
	stlinkutil_targets()
	stflash_targets()
	cubeprog_targets()	
	openocd_targets()
else()
	if(DEFAULT_PROGRAMMER STREQUAL "STM32CUBEPROG")
		blackmagic_targets()
		stlinkutil_targets()
		stflash_targets()
		openocd_targets()
		cubeprog_targets()	
	elseif(DEFAULT_PROGRAMMER STREQUAL "OPENOCD")
		blackmagic_targets()
		stlinkutil_targets()
		stflash_targets()
		cubeprog_targets()	
		openocd_targets()
	elseif(DEFAULT_PROGRAMMER STREQUAL "STFLASH" OR DEFAULT_PROGRAMMER STREQUAL "TEXANE")
		blackmagic_targets()
		stlinkutil_targets()
		openocd_targets()
		cubeprog_targets()	
		stflash_targets()
	elseif(DEFAULT_PROGRAMMER STREQUAL "STLINKUTILITY")
		blackmagic_targets()
		stflash_targets()
		openocd_targets()
		cubeprog_targets()	
		stlinkutil_targets()
	endif()
endif()

if (DEFINED FLASH_TARGET)
	add_custom_target(flash DEPENDS ${FLASH_TARGET})
	message("flash using ${FLASH_TARGET}")
else()
	message("No program found for flash")
endif()
if (DEFINED ERASE_TARGET)
	add_custom_target(erase DEPENDS ${ERASE_TARGET})
	message("erase using ${ERASE_TARGET}")
else()
	message("No program found for erase")
endif()
if (DEFINED DEBUG_TARGET)
	add_custom_target(debug DEPENDS ${DEBUG_TARGET})
	message("debug using ${DEBUG_TARGET}")
else()
	message("No program found for debug")
endif()
if (DEFINED GDB_TARGET)
	add_custom_target(gdb DEPENDS ${GDB_TARGET})
	message("gdb using ${GDB_TARGET}")
else()
	message("No program found for gdb")
endif()

