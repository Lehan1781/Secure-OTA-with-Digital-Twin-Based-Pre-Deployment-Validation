@echo off
echo ==========================
echo Building STM32 Firmware
echo ==========================

if not exist build (
    mkdir build
)

cd build

cmake -G "MinGW Makefiles" ^
-DCMAKE_TOOLCHAIN_FILE=../toolchain-arm-none-eabi.cmake ^
..

mingw32-make

cd ..

echo ==========================
echo Build Complete
echo ==========================
pause
