obj-m += filter.o

modules:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean

ins:
	sudo insmod filter.ko

rm:
	sudo rmmod filter