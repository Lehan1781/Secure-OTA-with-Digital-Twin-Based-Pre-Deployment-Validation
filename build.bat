@echo off

if not exist build (
    mkdir build
)

cd build

cmake -G "MinGW Makefiles" ^
 -DCMAKE_TOOLCHAIN_FILE=../toolchain-arm-none-eabi.cmake^
 ..

cmake --build . -- -j

echo ==========================
echo BUILD COMPLETE WOOHOOO
echo ==========================

pause
