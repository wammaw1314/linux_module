ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../mpp/Makefile.param
    include $(PARAM_FILE)
endif

CC=$(CROSS)gcc

EXTRA_CFLAGS += -D$(HI_FPGA)

STRIP=$(CROSS)strip
LDFLAGS+=--strip-debug
obj-m := cdemo.o
cdemo-y := demo.o

.PHONY: all clean

all: ko

ko:
	@make -C $(LINUX_ROOT) M=$(PWD) modules
	@$(STRIP) $(LDFLAGS) *.ko
clean:
	@make -C $(LINUX_ROOT) M=$(PWD) clean