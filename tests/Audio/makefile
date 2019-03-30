# Created by the Intel FPGA Monitor Program
# DO NOT MODIFY

############################################
# Global Defines
DEFINE_COMMA	:= ,

############################################
# Compilation Targets

# Programs
AS		:= arm-altera-eabi-as
CC		:= arm-altera-eabi-gcc
LD		:= arm-altera-eabi-ld
OC		:= arm-altera-eabi-objcopy
RM		:= rm -f

# Flags
USERCCFLAGS	:= -g -O1
ARCHASFLAGS	:= -mfloat-abi=soft -march=armv7-a -mcpu=cortex-a9 --gstabs -I "$$GNU_ARM_TOOL_ROOTDIR/arm-altera-eabi/include/"
ARCHCCFLAGS	:= -mfloat-abi=soft -march=armv7-a -mtune=cortex-a9 -mcpu=cortex-a9
ARCHLDFLAGS	:= --defsym arm_program_mem=0x0 --defsym arm_available_mem_size=0x3ffffff8 --defsym __cs3_stack=0x3ffffff8
ARCHLDSCRIPT	:= -T"C:/intelFPGA_lite/18.0/University_Program/Monitor_Program/build/altera-socfpga-hosted.ld"
ASFLAGS		:= $(ARCHASFLAGS) -lm
CCFLAGS		:= -Wall -c $(USERCCFLAGS) $(ARCHCCFLAGS)
LDFLAGS		:= $(patsubst %, -Wl$(DEFINE_COMMA)%, $(ARCHLDFLAGS)) $(ARCHLDSCRIPT)
OCFLAGS		:= -O srec

# Files
HDRS		:= address_map_arm.h
SRCS		:= testaud.c 
OBJS		:= $(patsubst %, %.o, $(SRCS))

# Targets
compile: testaud.srec

testaud.srec: testaud.axf
	$(RM) $@
	$(OC) $(OCFLAGS) $< $@

testaud.axf: $(OBJS)
	$(RM) $@
	$(CC) $(LDFLAGS) $(OBJS) -lm -o $@

%.c.o: %.c $(HDRS)
	$(RM) $@
	$(CC)  $(CCFLAGS) $< -lm -o $@

%.s.o: %.s $(HDRS)
	$(RM) $@
	$(AS) $(ASFLAGS) $< -o $@

clean:
	$(RM) testaud.srec testaud.axf $(OBJS)

