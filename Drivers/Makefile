obj-m += tmp100-25-11.o
 
KDIR = ~/Documents/Work/linux-src/linux
all:
	make -C $(KDIR)  M=$(shell pwd) modules
 
clean:
	make -C $(KDIR)  M=$(shell pwd) clean
