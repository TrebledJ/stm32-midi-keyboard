##############################################################################################################################################################################
# This section is parsing the STM32FXXX model no you gave me to find related things automatically
# Its real jank LMAO

# I always generate lower case versions of everything,
# This step also normalizes the model number to all uppercase
string(TOLOWER ${STM32FXXX} stm32fxxx)
string(TOUPPER ${STM32FXXX} STM32FXXX)
message("STM32Fxyzab: ${STM32FXXX}")

# Determine STM32FXXX family, This is the first digit of the MCU (just look at the regex la)
string(REGEX MATCH "^(STM32[FLHG][0-9])" STM32_FAMILY "${STM32FXXX}")
string(TOLOWER ${STM32_FAMILY} stm32_family)
message("STM32Fx Family: ${STM32_FAMILY}")

# Determine STM32FXXX series, This is the first 3 digits
string(REGEX MATCH "^(STM32[FLHG][0-9][0-9][0-9])" STM32_SERIES "${STM32FXXX}")
message("STM32Fxxx Series: ${STM32_SERIES}")
string(TOLOWER ${STM32_SERIES} stm32_series)

# Use Regex to find the 'series' of MCU
# this is like: STM32F4x9 and STM32F10x
# STMicro is crazy inconsistent with how they name the MCUs and the associated files, 
# so i have to basically try every fucking combination 
string(REGEX MATCH "^(STM32[FLHG][0-9][0-9][0-9])([A-Z])([A-Z0-9])" CPU_FAMILY_MATCH "${STM32FXXX}")
set(STM32_SERIES_A "${CMAKE_MATCH_1}x${CMAKE_MATCH_3}")
set(STM32_SERIES_B "${CMAKE_MATCH_1}${CMAKE_MATCH_2}x")
string(TOLOWER ${STM32_SERIES_A} stm32_series_a)
string(TOLOWER ${STM32_SERIES_B} stm32_series_b)
message("${STM32_SERIES_A}/${STM32_SERIES_B}")

# Set CPU type for compiler
# This is the Cortex Mx series, aka the actual ARM core inside the MCU
# TODO: This doesnt account for all MCUs yet, just the only ones we use, it throws an error if i've never seen it before
# I also define some stuff about what kind of FPU support this core has
if(${STM32_FAMILY} STREQUAL "STM32F1")
	set(GCC_CPU "cortex-m3")
	set(ARMCC_CPU "Cortex-M3")
	set(DEF_ARM_MATH -DARM_MATH_CM3 -Dnum_t=float)
	set(HAS_FPU 0)
elseif(${STM32_FAMILY} STREQUAL "STM32F4" OR ${STM32_FAMILY} STREQUAL "STM32F3")
	set(GCC_CPU "cortex-m4")
	set(ARMCC_CPU "Cortex-M4.fp")
	set(DEF_ARM_MATH -DARM_MATH_CM4 -Dnum_t=float -DHAS_FLOAT)
	set(HAS_FPU 1)
	set(GCC_FPU fpv4-sp-d16)
	set(SP_FPU 1)
elseif(${STM32_FAMILY} STREQUAL "STM32H7")
	set(GCC_CPU "cortex-m7")
	set(ARMCC_CPU "Cortex-M7.fp.dp")
	set(DEF_ARM_MATH -DARM_MATH_CM7 -Dnum_t=double -DHAS_DOUBLE)
	set(HAS_FPU 1)
	set(GCC_FPU fpv5-d16)
elseif(${STM32_FAMILY} STREQUAL "STM32F0")
	set(GCC_CPU "cortex-m0")
	set(ARMCC_CPU "Cortex-M0")
	set(DEF_ARM_MATH -DARM_MATH_CM0 -Dnum_t=float)
	set(HAS_FPU 0)
else()
	message(FATAL_ERROR "Unrecognised device family: ${STM32_FAMILY}")
endif()

# As I said STMicro is inconsistent with how they name things, 
# so the following is a hardcoded mapping of MCU to what defines have to exist for their hw libraries to work.
# This is found through trial and pain and error
set(STM32_DEFINES ${DEF_ARM_MATH} 
	-Dstm32header="${stm32_family}xx.h" -DSTM32_MCU=${STM32FXXX} -DSTM32_SERIES=${STM32_SERIES} -DSTM32_FAMILY=${STM32_FAMILY}
	-D${STM32FXXX} -D${STM32_SERIES}xx -D${STM32_FAMILY} -D${STM32_SERIES_A} -D${STM32_SERIES_B} -D${STM32_SERIES_A} -D${STM32_SERIES_B})
if (STM32_SERIES STREQUAL STM32F405)
	list(APPEND STM32_DEFINES -DSTM32F40_41xxx)
elseif (STM32_SERIES STREQUAL STM32F415)
	list(APPEND STM32_DEFINES -DSTM32F40_41xxx)
elseif (STM32_SERIES STREQUAL STM32F407)
	list(APPEND STM32_DEFINES -DSTM32F40_41xxx)
elseif (STM32_SERIES STREQUAL STM32F417)
	list(APPEND STM32_DEFINES -DSTM32F40_41xxx)
elseif (STM32_SERIES STREQUAL STM32F427)
	list(APPEND STM32_DEFINES -DSTM32F427_437xx)
elseif (STM32_SERIES STREQUAL STM32F437)
	list(APPEND STM32_DEFINES -DSTM32F427_437xx)
elseif (STM32_SERIES STREQUAL STM32F429)
	list(APPEND STM32_DEFINES -DSTM32F429_439xx)
elseif (STM32_SERIES STREQUAL STM32F439)
	list(APPEND STM32_DEFINES -DSTM32F429_439xx)
elseif (STM32_SERIES STREQUAL STM32F401) #xC
elseif (STM32_SERIES STREQUAL STM32F401) #xE
elseif (STM32_SERIES STREQUAL STM32F410) #Tx
elseif (STM32_SERIES STREQUAL STM32F410) #Cx
elseif (STM32_SERIES STREQUAL STM32F410) #Rx
elseif (STM32_SERIES STREQUAL STM32F411) #xE
elseif (STM32_SERIES STREQUAL STM32F446)
elseif (STM32_SERIES STREQUAL STM32F469)
	list(APPEND STM32_DEFINES -DSTM32F469_479xx)
elseif (STM32_SERIES STREQUAL STM32F479)
	list(APPEND STM32_DEFINES -DSTM32F469_479xx)
elseif (STM32_SERIES STREQUAL STM32F412) #Cx
elseif (STM32_SERIES STREQUAL STM32F412) #Zx
elseif (STM32_SERIES STREQUAL STM32F412) #Rx
elseif (STM32_SERIES STREQUAL STM32F412) #Vx
elseif (STM32_SERIES STREQUAL STM32F413)
elseif (STM32_SERIES STREQUAL STM32F423)
endif()
