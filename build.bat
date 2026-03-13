@echo off
set BUILD_DIR=build

echo ======================================
echo STM32 OTA Bootloader Build
echo ======================================

REM --- CLEAN BUILD DIRECTORY EVERY TIME ---
if exist %BUILD_DIR% (
    echo Cleaning previous build...
    rmdir /s /q %BUILD_DIR%
)

mkdir %BUILD_DIR%
cd %BUILD_DIR%

cmake -G "MinGW Makefiles" ..
if errorlevel 1 goto error

cmake --build .
if errorlevel 1 goto error

echo.
echo ======================================
echo BUILD SUCCESS
echo ======================================
echo Output files:
echo   build\bootloader.elf
echo   build\bootloader.hex
echo   build\bootloader.bin
goto end

:error
echo.
echo ======================================
echo BUILD FAILED
echo ======================================
exit /b 1

:end
cd ..
pause
