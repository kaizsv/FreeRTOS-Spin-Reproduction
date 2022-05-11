BUILD_DIR = build
FREERTOS_KERNEL = FreeRTOS-Kernel
STM32CUBEF4 = STM32CubeF4
DRIVER = $(STM32CUBEF4)/Drivers/STM32F4xx_HAL_Driver
CMSIS = $(STM32CUBEF4)/Drivers/CMSIS
BSP_TARGET = $(STM32CUBEF4)/Drivers/BSP/STM32F429I-Discovery

COMPILER = arm-none-eabi
CC = $(COMPILER)-gcc
LD = $(COMPILER)-ld
AS = $(COMPILER)-as
OBJCOPY = $(COMPILER)-objcopy
OBJDUMP = $(COMPILER)-objdump
SIZE = $(COMPILER)-size
GDB = gdb-multiarch

MCU = -mcpu=cortex-m4 -march=armv7e-m -mtune=cortex-m4
CFLAGS = -ggdb3 -O0 -std=c99 -W -Wall $(MCU) \
		 -mlittle-endian -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp \
		 -DUSE_HAL_DRIVER -DSTM32F429xx -DVECT_TAB_FLASH \
		 -fdata-sections -ffunction-sections -fno-common
LIBS = -lc -lgcc -lg
LDFLAGS = -Wl,--gc-sections $(LIBS)
define get_library_path
	$(shell dirname $(shell $(CC) $(CFLAGS) -print-file-name=$(1)))
endef
LDFLAGS += -L $(call get_library_path,libc.a)
LDFLAGS += -L $(call get_library_path,libgcc.a)

LIB_SSRC = $(CMSIS)/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f429xx.s

LIB_CINC = -I$(BSP_TARGET) \
		   -I$(CMSIS)/Include \
		   -I$(CMSIS)/Device/ST/STM32F4xx/Include \
		   -I$(DRIVER)/Inc \
		   -I$(FREERTOS_KERNEL)/include \
		   -I$(FREERTOS_KERNEL)/portable/GCC/ARM_CM4F \

LIB_CSRC = $(DRIVER)/Src/stm32f4xx_hal.c \
		   $(DRIVER)/Src/stm32f4xx_hal_gpio.c \
		   $(DRIVER)/Src/stm32f4xx_hal_cortex.c \
		   $(DRIVER)/Src/stm32f4xx_hal_rcc.c \
		   $(DRIVER)/Src/stm32f4xx_hal_pwr_ex.c \
		   $(BSP_TARGET)/stm32f429i_discovery.c \
		   $(CMSIS)/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c \
		   $(FREERTOS_KERNEL)/list.c \
		   $(FREERTOS_KERNEL)/tasks.c \
		   $(FREERTOS_KERNEL)/queue.c \
		   $(FREERTOS_KERNEL)/timers.c \
		   $(FREERTOS_KERNEL)/portable/GCC/ARM_CM4F/port.c \
		   $(FREERTOS_KERNEL)/portable/MemMang/heap_4.c \

LIB_OBJS = $(LIB_SSRC:.s=.o) $(LIB_CSRC:.c=.o)
VPATH = $(dir $(LIB_OBJS))
BUILD_OBJS = $(patsubst %.o, build/%.o, $(notdir $(LIB_OBJS)))
