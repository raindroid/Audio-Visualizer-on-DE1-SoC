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
USERCCFLAGS	:= -g -O0 -std=c11
ARCHASFLAGS	:= -mfloat-abi=softfp -march=armv7-a -mcpu=cortex-a9 --gstabs -I "$$GNU_ARM_TOOL_ROOTDIR/arm-altera-eabi/include/"
ARCHCCFLAGS	:= -mfloat-abi=softfp -march=armv7-a -mtune=cortex-a9 -mcpu=cortex-a9
ARCHLDFLAGS	:= --defsym arm_program_mem=0x0 --defsym arm_available_mem_size=0x3ffffff8 --defsym __cs3_stack=0x3ffffff8
ARCHLDSCRIPT	:= -T"C:/intelFPGA_lite/18.1/University_Program/Monitor_Program/build/altera-socfpga-hosted.ld"
ASFLAGS		:= $(ARCHASFLAGS)
CCFLAGS		:= -Wall -c $(USERCCFLAGS) $(ARCHCCFLAGS)
LDFLAGS		:= $(patsubst %, -Wl$(DEFINE_COMMA)%, $(ARCHLDFLAGS)) $(ARCHLDSCRIPT)
OCFLAGS		:= -O srec

# Files
HDRS		:= address_map_arm.h values.h lib/JTAG_UART.h lib/Audio.h lib/LED.h lib/HEX.h  tests/tests.h views/VGA_Display.h helpers/Math.h models/FFT.h
SRCS		:= main.c values.c lib/JTAG_UART.c lib/Audio.c lib/LED.c lib/HEX.c tests/lib_tests.c views/VGA_Display.c helpers/Math.c models/FFT.c
OBJS		:= $(patsubst %, %.o, $(SRCS))

# Targets
compile: main.srec

main.srec: main.axf
	$(RM) $@
	$(OC) $(OCFLAGS) $< $@

main.axf: $(OBJS)
	$(RM) $@
	$(CC) $(LDFLAGS) $(OBJS) -o $@ -lm

%.c.o: %.c $(HDRS)
	$(RM) $@
	$(CC) $(CCFLAGS) $< -o $@ 

%.s.o: %.s $(HDRS)
	$(RM) $@
	$(AS) $(ASFLAGS) $< -o $@

clean:
	$(RM) main.srec main.axf $(OBJS)

