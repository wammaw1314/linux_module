CROSS=mips-linux-gnu-
CC=$(CROSS)gcc
LINUX_ROOT=../kernel
STRIP=$(CROSS)strip
LDFLAGS+=--strip-debug
obj-m := emulti.o
emulti-y := ethmulti.o

.PHONY: all clean

all: ko

ko:
	@make -C $(LINUX_ROOT) M=$(PWD) modules
	@$(STRIP) $(LDFLAGS) *.ko
clean:
	@make -C $(LINUX_ROOT) M=$(PWD) clean

