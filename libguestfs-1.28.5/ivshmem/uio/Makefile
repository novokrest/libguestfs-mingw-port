# Makefile for libguestfs-mingw-port: uio module

# obj-m is a list of what kernel modules to build.  The .o and other
# objects will be automatically built from the corresponding .c file -
# no need to list the source files explicitly.

obj-m := uio_ivshmem.o 

# KDIR is the location of the kernel source.  The current standard is
# to link to the associated source tree from the directory containing
# the compiled modules.
KDIR  := /lib/modules/$(shell uname -r)/build

# PWD is the current working directory and the location of our module
# source files.
PWD   := $(shell pwd)

# default is the default make target.  The rule here says to run make
# with a working directory of the directory containing the kernel
# source and compile only the modules in the PWD (local) directory.

all: default

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

install:
	sudo cp uio_ivshmem.ko /lib/modules/$(shell uname -r)/kernel/drivers/uio/
	sudo depmod -a	

uninstall:
	sudo rm -f /lib/modules/$(shell uname -r)/kernel/drivers/uio/uio_ivshmem.ko

clean:
	rm -f *~ *.o uio_ivshmem.mod.* Module.symvers modules.order .uio_ivshmem*
	rm -r .tmp_versions

.PHONY: all default install uninstall clean

