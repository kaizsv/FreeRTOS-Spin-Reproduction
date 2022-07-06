# FreeRTOS-Spin-Reproduction

## Quick Start

1. Install packages on Ubuntu
```
apt install gcc-arm-none-eabi gdb-multiarch openocd stlink-tools make
```

Update submodules
```
cd FreeRTOS-Spin-Reproduction
git submodule update --init --recursive
```

2. Compile an application and flash binary to stm32f429
```
cd <<scheduling_policy>>/<<application>>
(optional) make clean-lib # Re-compile libraries if scheduling policy is changed.
make flash
```

Be sure to `make clean-lib` after scheduling policy is changed.

3. Try to add plug tasks. Plug tasks are derived from the model checking
results in [FreeRTOS-Spin](https://github.com/kaizsv/FreeRTOS-Spin).
```
make clean
make PLUG=<<a_plug_in_plug_folder>>
make flash
```

Similary, some applications have a `fix` folder.
```
make clean && make FIX=<<a_fix_in_fix_folder>>
```

4. To debug an application, run `make openocd` and `make gdb` in separated
terminals.

Note: If `openocd` crashes while executing, try the following script.
```
apt autoremove --purge openocd
git clone https://git.code.sf.net/p/openocd/code openocd-code
cd openocd-code
./bootstrap
./configure --enable-stlink
make
make install
```

