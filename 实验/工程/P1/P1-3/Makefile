TARGET = P1-3
KDIR = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd)
obj-m += $(TARGET).o
default:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf *.o *.mod.c *.cmd *.markers *.order *.symvers *.tmp_versions