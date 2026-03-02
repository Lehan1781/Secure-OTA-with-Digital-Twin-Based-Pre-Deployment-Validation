cmake_minimum_required(VERSION 3.20)

project(OTA_Project C ASM)

set(CMAKE_C_STANDARD 11)

add_compile_definitions(
    STM32F446xx
    USE_HAL_DRIVER
)

set(MCU_FLAGS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -ffunction-sections
    -fdata-sections
)

set(COMMON_INCLUDES
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
)

file(GLOB HAL_SOURCES
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/*.c
)

# =========================
# BOOTLOADER
# =========================

add_executable(bootloader.elf
    Bootloader/startup_stm32f446retx.s
    Bootloader/boot_main.c
    Bootloader/system_stm32f4xx.c
    ${HAL_SOURCES}
)


target_include_directories(bootloader.elf PRIVATE
    ${COMMON_INCLUDES}
    bootloader
)

target_compile_options(bootloader.elf PRIVATE ${MCU_FLAGS})

target_link_options(bootloader.elf PRIVATE
    ${MCU_FLAGS}
    -T ${CMAKE_SOURCE_DIR}/bootloader/BOOT.ld
    -Wl,--gc-sections
    --specs=nano.specs
    --specs=nosys.specs
)

# =========================
# APP
# =========================

add_executable(app.elf
    app/startup_stm32f446retx.s
    app/app_main.c
    app/system_stm32f4xx.c
    ${HAL_SOURCES}
)

target_include_directories(app.elf PRIVATE
    ${COMMON_INCLUDES}
    app
)

target_compile_options(app.elf PRIVATE ${MCU_FLAGS})

target_link_options(app.elf PRIVATE
    ${MCU_FLAGS}
    -T ${CMAKE_SOURCE_DIR}/app/APP.ld
    -Wl,--gc-sections
    --specs=nano.specs
    --specs=nosys.specs
)
