cmake_minimum_required(VERSION 3.20)

# Tell CMake this is bare-metal, not Windows
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m4)
set(CMAKE_EXECUTABLE_SUFFIX "")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(bootloader C ASM)

# -------------------------------------------------
# Toolchain (explicit, no auto-detection)
# -------------------------------------------------
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_SIZE arm-none-eabi-size)

# -------------------------------------------------
# CPU / FPU flags (STM32F446RE – Cortex-M4F)
# -------------------------------------------------
set(MCU_FLAGS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

# -------------------------------------------------
# Compile options (C + ASM)
# -------------------------------------------------

add_compile_definitions(STM32F446xx)

add_compile_options(
    ${MCU_FLAGS}
    -Wall
    -Wextra
    -Wno-unused-parameter
    -O0
    -g3
)

# -------------------------------------------------
# Link options
# -------------------------------------------------
add_link_options(
    ${MCU_FLAGS}
    -T${CMAKE_SOURCE_DIR}/STM32F446RETX_FLASH.ld
    -Wl,--gc-sections
    -Wl,-Map=bootloader.map
)

# -------------------------------------------------
# Source files (explicit)
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

set(HAL_SOURCES
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_wwdg.c
)

# -------------------------------------------------
# Startup file (ONLY ASM file)
# -------------------------------------------------
set(STARTUP_FILE
    ${CMAKE_SOURCE_DIR}/Core/Startup/startup_stm32f446retx.s
)

# Tell CMake how to compile this ASM file
set_source_files_properties(
    ${STARTUP_FILE}
    PROPERTIES
    COMPILE_FLAGS "-x assembler-with-cpp"
)

# -------------------------------------------------
# Include directories
# -------------------------------------------------
include_directories(
    Core/Inc
    Drivers/STM32F4xx_HAL_Driver/Inc
    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    Drivers/CMSIS/Include
)

# -------------------------------------------------
# Executable
# -------------------------------------------------
add_executable(bootloader.elf
    ${APP_SOURCES}
    ${HAL_SOURCES}
    ${STARTUP_FILE}
)

# -------------------------------------------------
# Post-build outputs
# -------------------------------------------------
add_custom_command(TARGET bootloader.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex bootloader.elf bootloader.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary bootloader.elf bootloader.bin
    COMMAND ${CMAKE_SIZE} bootloader.elf
)
