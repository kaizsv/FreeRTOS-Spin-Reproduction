PROGRAM = main
LIB_DIR = ../../lib
PLAT = stm32f429
include $(LIB_DIR)/$(PLAT).mk

ifdef PLUG
	CFLAGS += -DUSE_PLUG
endif

LDFLAGS += -T$(LIB_DIR)/$(STM32CUBEF4)/Projects/STM32F429I-Discovery/$\
		   Templates/STM32CubeIDE/STM32F429ZITX_FLASH.ld

CINC = -I. -I./inc $(subst -I, -I$(LIB_DIR)/, $(LIB_CINC))
CSRC = $(wildcard *.c)
OBJS = $(CSRC:.c=.o)
LIB_BUILD_OBJS = $(addprefix $(LIB_DIR)/, $(BUILD_OBJS))

all: $(PROGRAM).bin

%.o: %.c
	$(CC) -o $@ $(CINC) $(CFLAGS) -c $<

$(LIB_BUILD_OBJS):
	$(MAKE) -C $(LIB_DIR) PLAT=$(PLAT) \
		APP_INCLUDE_PATH="-I$(shell pwd) -I$(shell pwd)/inc"

$(PROGRAM).elf: $(OBJS) $(LIB_BUILD_OBJS)
	$(CC) -o $@ $(MCU) $(LDFLAGS) $^

$(PROGRAM).bin: $(PROGRAM).elf
	$(OBJCOPY) -O binary $< $@
	$(OBJCOPY) -O ihex $< $(PROGRAM).hex
	$(OBJDUMP) -h -S -D $< > $(PROGRAM).lst
	$(SIZE) $<

.PHONY: flash
flash: all
	st-flash --reset write $(PROGRAM).bin 0x8000000

.PHONY: openocd
openocd:
	openocd -f board/stm32f429disc1.cfg	\
		-c "init" \
		-c "reset init" \
		-c "arm semihosting enable" \
		-c "reset run"

.PHONY: gdb
gdb: $(PROGRAM).elf
	$(GDB) -q $< -ex "target remote :3333"

.PHONY: clean clean-lib
clean:
	-rm *.o *.elf *.bin *.lst *.hex

clean-lib:
	$(MAKE) -C $(LIB_DIR) clean
