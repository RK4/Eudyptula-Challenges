CFLAGS_hello_world.o := -DDEBUG
obj-m += hello_world.o
KDIR := /lib/modules/$(shell uname -r)/build


all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
