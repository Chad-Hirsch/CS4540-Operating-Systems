# An LKM Makefile
#
# A Makefile is required to build a loadable kernel module (LKM).  In fact, it is a special kbuild
# Makefile.
# goal definition, defines the module to be built (bug.o)
# obj-m defines a loadable module goal, whereas obj-y indicates a built-in object goal
obj-m+=bug.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
test:
	date
	sudo dmesg -C
	sudo insmod bug.ko
	sudo rmmod bug.ko
	dmesg
	date
