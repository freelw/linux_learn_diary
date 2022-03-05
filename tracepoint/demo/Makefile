
MODULE_NAME := tracepoint_demo
KVERS = $(shell uname -r)
obj-m += ${MODULE_NAME}.o
CFLAGS_tracepoint_demo.o = -I$(src)

build: kernel_modules

kernel_modules:
		make -C /lib/modules/$(KVERS)/build M=$(CURDIR) modules

clean:
		make -C /lib/modules/$(KVERS)/build M=$(CURDIR) clean

install:
	insmod $(MODULE_NAME).ko

remove:
	rmmod $(MODULE_NAME)
