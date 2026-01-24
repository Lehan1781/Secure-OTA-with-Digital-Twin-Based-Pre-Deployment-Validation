@echo off

set ELF_FILE=build\bootloader.elf

echo ======================================
echo Flashing STM32 Bootloader
echo ======================================

if not exist %ELF_FILE% (
    echo ERROR: ELF file not found.
    echo Please run build.bat first.
    exit /b 1
)

openocd ^
-f interface/stlink.cfg ^
-f target/stm32f4x.cfg ^
-c "program %ELF_FILE% verify reset exit"

if errorlevel 1 goto error

echo.
echo ======================================
echo FLASH SUCCESSFUL
echo ======================================
goto end

:error
echo.
echo ======================================
echo FLASH FAILED
echo ======================================
exit /b 1

:end
pause
