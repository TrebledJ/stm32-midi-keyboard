add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${OBJCOPY} -Oihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMENT "Building HEX File")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${OBJCOPY} -Obinary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMENT "Building BIN File")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${OBJDUMP} -d -r -S -l -w -marmv7 ${PROJECT_NAME}.elf > ${PROJECT_NAME}.lst
    COMMENT "Dumping Assembly Listing")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${OBJDUMP} -d -S -marmv7 ${PROJECT_NAME}.elf > ${PROJECT_NAME}.s
    COMMENT "Dumping Assembly Listing")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${OBJSIZE} ${PROJECT_NAME}.elf
    COMMENT "Object Size:")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${OBJSIZE} -A -d ${PROJECT_NAME}.elf
    COMMENT "Object Size:")
