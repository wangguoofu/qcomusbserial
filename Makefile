obj-m := qcgeneric.o #qcserial.o                   #要生成的模块名     
modules-objs:= qcgeneric.o #qcserial.o usb_wwan.o      #生成这个模块名所需要的目标文件

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	make -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.o .* .cmd *.ko *.mod.c .tmp_versions
