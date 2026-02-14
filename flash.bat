@echo off
echo ==========================
echo Flashing STM32
echo ==========================

cd build

openocd ^
-f interface/stlink.cfg ^
-f target/stm32f4x.cfg ^
-c "program firmware.elf verify reset exit"

cd ..

echo ==========================
echo Flash Complete
echo ==========================
pause
