ifeq ($(PLAT), stm32f429)
	include stm32f429.mk
ifndef APP_INCLUDE_PATH
	$(error)
endif
	LIB_CINC += $(APP_INCLUDE_PATH)
endif

all: BUILD_DIR $(BUILD_OBJS)

build/%.o: %.s
	$(CC) -o $@ $(CFLAGS) -c $<

build/%.o: %.c
	$(CC) -o $@ $(LIB_CINC) $(CFLAGS) -c $<

BUILD_DIR:
	mkdir -p build

.PHONY: clean
clean:
	-rm build/*
	-rmdir build
