##############################################################################################################################################################################

## Create Executable
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

if ("${CMAKE_C_COMPILER}" MATCHES "clang")
    set(USING_CLANG 1)
endif()

message("Generating executable: ${PROJECT_NAME} for ${STM32FXXX}-${CMAKE_BUILD_TYPE}")

add_executable(${PROJECT_NAME} ${MCU_STARTUP_FILE} ${SOURCE_FILES})

	set(COMP_DEFINES -D__pure=__attribute__\(\(const\)\) -D__forceinline=__attribute__\(\(always_inline\)\) -D__value_in_regs= -D__weak=__attribute__\(\(weak\)\) -D__packed=__attribute__\(\(__packed__\)\))

	set(COMPILER_INFO_FLAGS -g3)
	set(TIME_OPT_FLAGS -O3) 
	if(USE_LTO)
		list(APPEND TIME_OPT_FLAGS -flto -Wl,--undefined=vTaskSwitchContext)
		list(APPEND LINKER_OPT_FLAGS -flto -Wl,--undefined=vTaskSwitchContext)
	endif()
	if(USE_OFAST)
		list(APPEND TIME_OPT_FLAGS -Ofast)
	endif()
	set(SPACE_OPT_FLAGS -Os -s)
	set(DEBUG_OPT_FLAGS -Og)

	set(COMMON_FLAGS -mcpu=${GCC_CPU} -mthumb -fno-math-errno -fdiagnostics-color)
	if(NOT DEFINED EN_STRICT_ALIASING OR STRICT_ALIASING EQUAL 0)
		list(APPEND COMMON_FLAGS -fno-strict-aliasing)
	endif()
	if (DEFINED GCC_FPU)
		list(APPEND COMMON_FLAGS -mfloat-abi=hard -mfpu=${GCC_FPU})
	else()
		list(APPEND COMMON_FLAGS -mfpu=vfp -msoft-float)
	endif()
	if(NOT DEFINED GCC_SPECS)
		if (DEFINED CUBE_PROJECT OR DEFINED CUBE_PROJECT_LL)
			list(APPEND COMMON_FLAGS -specs=nosys.specs -specs=nano.specs)
		else()
			list(APPEND COMMON_FLAGS -specs=nosys.specs)
		endif()
	else()
		list(APPEND COMMON_FLAGS -specs=${GCC_SPECS})
	endif()

	# -Werror=cast-qual
	set(GCC_WARN_FLAGS_C -Werror=discarded-qualifiers -Werror=incompatible-pointer-types -Werror=implicit-int -Werror=int-conversion -Werror=implicit-function-declaration)
	set(GCC_WARN_FLAGS -Wall -Werror=restrict -Werror=uninitialized -Wno-comment 
        -Werror=return-type -Werror=cast-qual -Wno-unused-parameter -Wno-attributes 
        -Wno-unused-function -Wno-unknown-pragmas)
    if (USING_CLANG)
        # list(APPEND GCC_WARN_FLAGS -Werror=implicit-fallthrough) # FIXME: clang doesnt recognize comments
    else()
        list(APPEND GCC_WARN_FLAGS -Werror=implicit-fallthrough=3) # FIXME: Only in GCC
    endif()
	if (GCC_FATAL_ERRORS)
		list(APPEND GCC_WARN_FLAGS -Wfatal-errors)
	endif()
	# fshort-enums, -fdollars-in-identifiers
    # set(GCC_FEAT_FLAGS_C -fplan9-extensions) # Not supported in clang
	set(GCC_FEAT_FLAGS_C -fms-extensions)
	set(GCC_FEAT_FLAGS -g -fverbose-asm -fshort-enums #[[-fno-builtin-malloc]] -ffunction-sections -fdata-sections -fomit-frame-pointer -fno-common)
    if (NOT USING_CLANG)
        list(APPEND GCC_FEAT_FLAGS -freg-struct-return) # FIXME: Enable only when using GCC
    endif()
	if (SP_FPU)
		list(APPEND GCC_FEAT_FLAGS -fsingle-precision-constant -Werror=double-promotion)
	endif()
	set(COMPILER_FLAGS ${COMMON_FLAGS} ${GCC_WARN_FLAGS} ${GCC_FEAT_FLAGS})

	set(C_FLAGS ${GCC_WARN_FLAGS_C} ${GCC_FEAT_FLAGS_C} -Dauto=__auto_type)
	set(CPP_FLAGS -Wno-volatile -fconcepts-diagnostics-depth=9 -fno-exceptions -fno-non-call-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics  -fconcepts) #code sucks to much for -Weffc++

	set(LIST_CMAKE_C_FLAGS ${C_FLAGS} ${COMPILER_FLAGS})
	set(LIST_CMAKE_C_FLAGS_DEBUG ${DEBUG_OPT_FLAGS} ${COMPILER_INFO_FLAGS})
	set(LIST_CMAKE_C_FLAGS_MINSIZEREL ${SPACE_OPT_FLAGS})
	set(LIST_CMAKE_C_FLAGS_RELEASE ${TIME_OPT_FLAGS} -funroll-loops) #ftracer, fpeel-loops, -funswitch-loops
	set(LIST_CMAKE_C_FLAGS_RELWITHDEBINFO ${TIME_OPT_FLAGS} ${COMPILER_INFO_FLAGS})
	set(LIST_CMAKE_ASM_FLAGS ${COMMON_FLAGS})
	set(LIST_CMAKE_ASM_FLAGS_DEBUG)
	set(LIST_CMAKE_ASM_FLAGS_MINSIZEREL)
	set(LIST_CMAKE_ASM_FLAGS_RELEASE)
	set(LIST_CMAKE_ASM_FLAGS_RELWITHDEBINFO)
	set(LIST_CMAKE_CXX_FLAGS ${CPP_FLAGS} ${COMPILER_FLAGS})
	set(LIST_CMAKE_CXX_FLAGS_DEBUG ${DEBUG_OPT_FLAGS} ${COMPILER_INFO_FLAGS})
	set(LIST_CMAKE_CXX_FLAGS_MINSIZEREL ${SPACE_OPT_FLAGS})
	set(LIST_CMAKE_CXX_FLAGS_RELEASE ${TIME_OPT_FLAGS} -funroll-loops)
	set(LIST_CMAKE_CXX_FLAGS_RELWITHDEBINFO ${TIME_OPT_FLAGS} ${COMPILER_INFO_FLAGS})
	set(LIST_CMAKE_EXE_LINKER_FLAGS -Wl,-gc-sections -T \"${LINKER_SCRIPT}\" -Wl,-Map="${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map",--cref)
	set(LIST_CMAKE_EXE_LINKER_FLAGS_DEBUG ${LINKER_INFO_FLAGS})
	set(LIST_CMAKE_EXE_LINKER_FLAGS_MINSIZEREL )
	set(LIST_CMAKE_EXE_LINKER_FLAGS_RELEASE ${LINKER_OPT_FLAGS})
	set(LIST_CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO ${LINKER_INFO_FLAGS})

	# target_sources(${PROJECT_NAME} PUBLIC ${LINKER_SCRIPT})

	# get_target_property(_cur_link_deps ${PROJECT_NAME} LINK_DEPENDS)
    # string(APPEND _cur_link_deps ${LINKER_SCRIPT})
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_DEPENDS "${LINKER_SCRIPT}")

	set(ALL_DEFINES ${USER_DEFINES} ${COMP_DEFINES} ${STM32_DEFINES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    SUFFIX ".elf"
)

target_compile_definitions(${PROJECT_NAME} PUBLIC ${ALL_DEFINES})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES})

join("${LIST_CMAKE_C_FLAGS}" " " CMAKE_C_FLAGS)
join("${LIST_CMAKE_C_FLAGS_DEBUG}" " " CMAKE_C_FLAGS_DEBUG)
join("${LIST_CMAKE_C_FLAGS_MINSIZEREL}" " " CMAKE_C_FLAGS_MINSIZEREL)
join("${LIST_CMAKE_C_FLAGS_RELEASE}" " " CMAKE_C_FLAGS_RELEASE)
join("${LIST_CMAKE_C_FLAGS_RELWITHDEBINFO}" " " CMAKE_C_FLAGS_RELWITHDEBINFO)
join("${LIST_CMAKE_ASM_FLAGS}" " " CMAKE_ASM_FLAGS)
join("${LIST_CMAKE_ASM_FLAGS_DEBUG}" " " CMAKE_ASM_FLAGS_DEBUG)
join("${LIST_CMAKE_ASM_FLAGS_MINSIZEREL}" " " CMAKE_ASM_FLAGS_MINSIZEREL)
join("${LIST_CMAKE_ASM_FLAGS_RELEASE}" " " CMAKE_ASM_FLAGS_RELEASE)
join("${LIST_CMAKE_ASM_FLAGS_RELWITHDEBINFO}" " " CMAKE_ASM_FLAGS_RELWITHDEBINFO)
join("${LIST_CMAKE_CXX_FLAGS}" " " CMAKE_CXX_FLAGS)
join("${LIST_CMAKE_CXX_FLAGS_DEBUG}" " " CMAKE_CXX_FLAGS_DEBUG)
join("${LIST_CMAKE_CXX_FLAGS_MINSIZEREL}" " " CMAKE_CXX_FLAGS_MINSIZEREL)
join("${LIST_CMAKE_CXX_FLAGS_RELEASE}" " " CMAKE_CXX_FLAGS_RELEASE)
join("${LIST_CMAKE_CXX_FLAGS_RELWITHDEBINFO}" " " CMAKE_CXX_FLAGS_RELWITHDEBINFO)
join("${CMAKE_EXE_LINKER_FLAGS} ${LIST_CMAKE_EXE_LINKER_FLAGS}" " " CMAKE_EXE_LINKER_FLAGS)
join("${LIST_CMAKE_EXE_LINKER_FLAGS_DEBUG}" " " CMAKE_EXE_LINKER_FLAGS_DEBUG)
join("${LIST_CMAKE_EXE_LINKER_FLAGS_MINSIZEREL}" " " CMAKE_EXE_LINKER_FLAGS_MINSIZEREL)
join("${LIST_CMAKE_EXE_LINKER_FLAGS_RELEASE}" " " CMAKE_EXE_LINKER_FLAGS_RELEASE)
join("${LIST_CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}" " " CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO)

##############################################################################################################################################################################

if (CMAKE_HOST_WIN32)
	message("OS: Windows")
	add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
		COMMAND echo * > .gitignore)
else()
	message("OS: Not Windows")
	add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
		COMMAND touch .gitignore && echo '*' > .gitignore)
endif()

include(util_targets)
include(post_build)
