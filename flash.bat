@echo off
setlocal

:: -------------------------------
:: Select target to flash
:: -------------------------------
echo Select target to flash:
echo 1. Bootloader
echo 2. App Slot A
echo 3. App Slot B
set /p TARGET_CHOICE=Enter choice (1/2/3): 

if "%TARGET_CHOICE%"=="1" set ELF_FILE=build\bootloader.elf
if "%TARGET_CHOICE%"=="2" set ELF_FILE=build\app_slotA.elf
if "%TARGET_CHOICE%"=="3" set ELF_FILE=build\app_slotB.elf

:: -------------------------------
:: Flashing
:: -------------------------------
echo ======================================
echo Flashing %ELF_FILE%
echo ======================================

if not exist %ELF_FILE% (
    echo ERROR: ELF file not found.
    echo Please build the project first.
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
if "%TARGET_CHOICE%"=="2" (
    echo Partitioning done for Slot A
)
if "%TARGET_CHOICE%"=="3" (
    echo Partitioning done for Slot B
)
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
