This program is modified from the example FreeRTOS/Demo/CORTEX_M4F_STM32F407ZG-SK.

## Quick Start

1. Install packages on Ubuntu
```
apt install gcc-arm-none-eabi gdb-multiarch openocd stlink-tools make
```

2. Compile the program and flash the binary to STM32F429I board.
```
make flash
```

3. Add a plug task to starve a victim task in the `plug` floder.
```
make clean
make PLUG=[QProdB2|QConsB3] # either QProdB2 or QConsB3
```
After the binary file is flashed to a STM32F429I board, an error is indicated
by toggeling the on-board LED in higher frequence.
