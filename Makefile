# Makefile

# Kernel module details
obj-m += Temperature_Sensor.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

# User-space app
USER_APP := tempsensor_ui
USER_SRC := tempsensor_ncurses.c

.PHONY: all clean

all: kernel user

kernel:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

user:
	gcc -o $(USER_APP) $(USER_SRC) -lncurses

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f $(USER_APP)

