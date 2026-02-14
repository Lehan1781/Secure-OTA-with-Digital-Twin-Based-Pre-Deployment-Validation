cmake_minimum_required(VERSION 3.20)

project(firmware C ASM)

set(CMAKE_C_STANDARD 11)

add_compile_definitions(
    STM32F446xx
    USE_HAL_DRIVER
)

# ===============================
# INCLUDE DIRECTORIES
# ===============================

include_directories(
    Core/Inc
    Drivers/CMSIS/Include
    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    Drivers/STM32F4xx_HAL_Driver/Inc
)

# ===============================
# SOURCE FILES
# ===============================

file(GLOB_RECURSE CORE_SOURCES Core/Src/*.c)
file(GLOB_RECURSE HAL_SOURCES Drivers/STM32F4xx_HAL_Driver/Src/*.c)

set(SOURCES
    ${CORE_SOURCES}
    ${HAL_SOURCES}
    D:/OTA_Major_project/Core/Startup/startup_stm32f446retx.s
)

# ===============================
# CREATE EXECUTABLE FIRST
# ===============================

add_executable(firmware.elf ${SOURCES})

# ===============================
# >>> PASTE THE NEW FLAG SECTION HERE <<<
# ===============================

set(MCU_FLAGS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

target_compile_options(firmware.elf PRIVATE
    ${MCU_FLAGS}
     -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -std=gnu11
    -Wall
    -O0
    -ffunction-sections
    -fdata-sections
    -fno-exceptions
    -fno-unwind-tables
    -fno-asynchronous-unwind-tables
)


set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F446RETX_FLASH.ld)

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/STM32F446RETX_FLASH.ld")

target_link_options(firmware.elf PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -T "${LINKER_SCRIPT}"
    -Wl,--gc-sections
    -Wl,-Map=firmware.map
    --specs=nano.specs
    --specs=nosys.specs
)


target_link_libraries(firmware.elf PRIVATE
    -Wl,--gc-sections
    -Wl,-Map=firmware.map
)


# ===============================
# POST BUILD
# ===============================

add_custom_command(TARGET firmware.elf POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary firmware.elf firmware.bin
)

add_custom_command(TARGET firmware.elf POST_BUILD
    COMMAND arm-none-eabi-size firmware.elf
)
