#
# Author: jollen
# 
obj-m := cdata_dev_class.o

KDIR := /lib/modules/2.6.35-32-generic/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf *.o *.ko .*cmd modules.* Module.* .tmp_versions *.mod.c
