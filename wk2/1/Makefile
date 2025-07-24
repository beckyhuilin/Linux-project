CC_x86 := gcc
CC_arm := arm-linux-gnueabihf-gcc

CFLAGS :=-Wall

org := hello.c
obj_x86 := hello_x86.o
obj_arm := hello_arm.o

target_x86 := hello_x86
target_arm := hello_arm

all : x86 arm

x86 : $(obj_x86)
	$(CC_x86) $(CFLAGS) -o $(target_x86) $(obj_x86)

$(obj_x86) : $(org)
	$(CC_x86) $(CFLAGS) -c $(org) -o $(obj_x86)

arm : $(obj_arm)
	$(CC_arm) $(CFLAGS) -o $(target_arm) $(obj_arm)

$(obj_arm) : $(org)
	$(CC_arm) $(CFLAGS) -c $(org) -o $(obj_arm)

.PHONY : clean
clean:
	rm $(obj_x86) $(obj_arm) $(target_x86) $(target_arm)
	@echo "all cleaned up"

