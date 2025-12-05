ifneq ($(KERNELRELEASE),)
obj-m := ioctl-test.o
else
KDIR ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD
endif
