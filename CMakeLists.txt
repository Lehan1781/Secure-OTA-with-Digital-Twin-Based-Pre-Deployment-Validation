cmake_minimum_required(VERSION 3.20)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m4)
set(CMAKE_EXECUTABLE_SUFFIX "")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(bootloader C ASM)

# -------------------------------------------------
# Toolchain
# -------------------------------------------------
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_SIZE arm-none-eabi-size)

set(MCU_FLAGS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

add_compile_definitions(STM32F446xx)
add_compile_options(${MCU_FLAGS} -Wall -Wextra -Wno-unused-parameter -O0 -g3)

# -------------------------------------------------
# Source files
# -------------------------------------------------
set(APP_SOURCES
    Core/Src/main.c
    Core/Src/bootloader.c
    Core/Src/boot_metadata.c
    Core/Src/crc_check.c
    Core/Src/image_validate.c
    Core/Src/jump.c
    Core/Src/stm32f4xx_hal_msp.c
    Core/Src/stm32f4xx_it.c
    Core/Src/syscalls.c
    Core/Src/sysmem.c
    Core/Src/system_stm32f4xx.c
)

file(GLOB HAL_SOURCES
    "Drivers/STM32F4xx_HAL_Driver/Src/*.c"
)

set(STARTUP_FILE Core/Startup/startup_stm32f446retx.s)
set_source_files_properties(${STARTUP_FILE} PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp")

include_directories(
    Core/Inc
    Drivers/STM32F4xx_HAL_Driver/Inc
    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    Drivers/CMSIS/Include
)

# -------------------------------------------------
# Function to add firmware target with specific linker script
# -------------------------------------------------
function(add_firmware TARGET_NAME LINKER_SCRIPT)
    add_executable(${TARGET_NAME}.elf ${APP_SOURCES} ${HAL_SOURCES} ${STARTUP_FILE})
    target_compile_options(${TARGET_NAME}.elf PRIVATE ${MCU_FLAGS})
    target_link_options(${TARGET_NAME}.elf PRIVATE ${MCU_FLAGS} -T${LINKER_SCRIPT} -Wl,--gc-sections -Wl,-Map=${TARGET_NAME}.map)
    
    add_custom_command(TARGET ${TARGET_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${TARGET_NAME}.elf ${TARGET_NAME}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary ${TARGET_NAME}.elf ${TARGET_NAME}.bin
        COMMAND ${CMAKE_SIZE} ${TARGET_NAME}.elf
    )
endfunction()

# -------------------------------------------------
# Add three targets: bootloader, slot A, slot B
# -------------------------------------------------
add_firmware(bootloader ${CMAKE_SOURCE_DIR}/LinkerScripts/LinkerScript_bootloader.ld)
add_firmware(app_slotA  ${CMAKE_SOURCE_DIR}/LinkerScripts/LinkerScript_app_slotA.ld)
add_firmware(app_slotB  ${CMAKE_SOURCE_DIR}/LinkerScripts/LinkerScript_app_slotB.ld)
