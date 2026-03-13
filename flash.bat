@echo off

set PROJECT_DIR=%~dp0
set ELF_FILE=%PROJECT_DIR%build\bootloader.elf

REM Convert to forward slashes for OpenOCD
set ELF_FILE=%ELF_FILE:\=/%

echo ======================================
echo Flashing STM32 Bootloader
echo ======================================

if not exist "%PROJECT_DIR%build\bootloader.elf" (
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