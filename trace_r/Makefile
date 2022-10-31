MODULE_NAME := trace_r
  
obj-m += $(MODULE_NAME).o

$(MODULE_NAME)-objs := kp.o main.o

KERNEL_HEAD := $(shell uname -r)
KERNELDIR := /lib/modules/$(KERNEL_HEAD)/build
PWD := $(shell pwd)
ARCH := $(shell uname -m)
ifeq ($(ARCH),aarch64)
KCPPFLAGS="-mcmodel=large"
endif

all:
	KCPPFLAGS=$(KCPPFLAGS) $(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.ko *.mod *.mod.c *.o modules.* Module.symvers

install:
	insmod $(MODULE_NAME).ko

remove:
	rmmod $(MODULE_NAME)
                        
