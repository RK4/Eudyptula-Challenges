CFLAGS_hello_world.o := -DDEBUG
obj-m += hello_world.o
KDIR := /var/mentor/Drive/MyWork/teeny-linux/linux-4.10.6
SHELL := /bin/bash
PWD := $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
