obj-m := led.o
led-objs:=ledmodule.o utils.o
#KDIR := ~/buildroot-2023.02.8/output/build/linux-custom
KDIR := ~/buildroot-2023.02.8/output/build/linux-custom
ARCH ?= arm64
#arm64

#CROSS_COMPILE = /home/sofia/buildroot/buildroot-2021.08/output/host/bin/aarch64-linux-
#CROSS_COMPILE ?=/home/sofia/buildroot/buildroot-2021.08/output/host/bin/aarch64-buildroot-linux-gnu-

#~/buildroot/buildroot-2021.08/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-


CROSS_COMPILE ?= ~/buildroot-2023.02.8/output/host/bin/aarch64-buildroot-linux-gnu-
#CROSS_COMPILE ?= ~/buildroot-2023.02.8/output/host/bin/aarch64-linux- 

export ARCH CROSS_COMPILE


KBUILD_TARGETS	:= clean help modules modules_install

.PHONY: all $(KBUILD_TARGETS)

all: modules
$(KBUILD_TARGETS):
	#$(MAKE) -C $(KDIR) M=$(shell pwd) $@ EXTRA_CFLAGS="$(MY_CFLAGS)"
	$(MAKE) -C $(KDIR) M=$(shell pwd) $@
