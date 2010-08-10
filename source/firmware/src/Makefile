#
# Open-BLDC - Open BrushLess DC Motor Controller
# Copyright (c) 2009-2010 Piotr Esden-Tempski <piotr@esden.net>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. The name of the author may not be used to endorse or promote products
#    derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

NAME		?= open-bldc
PREFIX		?= arm-none-eabi
OOCD_INTERFACE	?= flossjtag
OOCD_TARGET	?= open-bldc

VERBOSE		?= 0

# Use 'make VERBOSE=1' for more debug output.
ifneq ($(VERBOSE),1)
Q := @
endif

CC		= $(PREFIX)-gcc
LD		= $(PREFIX)-ld
AR		= $(PREFIX)-ar
AS		= $(PREFIX)-as
CP		= $(PREFIX)-objcopy
OD		= $(PREFIX)-objdump
SIZE		= $(PREFIX)-size
OOCD		= openocd

TOOLCHAIN_LIB_DIR = `dirname \`which $(CC)\``/../$(PREFIX)/lib


INCDIRS		= -I. -Ipwm -Idriver -I../../libgovernor/include
CFLAGS          = $(INCDIRS) -Wall -ansi -std=c99 -c \
		  -fno-common -Os -g -mcpu=cortex-m3 -mthumb \
		  -ffunction-sections -fdata-sections
ASFLAGS         = -ahls -mapcs-32
LDFLAGS         = -Tstm32.ld -nostartfiles -L$(TOOLCHAIN_LIB_DIR) -Os \
		                    -L../../libgovernor/src/.libs --gc-sections
LDLIBS          = -lcmsis -lstm32 -lgovernor
CPFLAGS         = -j .isr_vector -j .text -j .data
ODFLAGS         = -S
SIZEFLAGS       = -A -x

DEFAULT_TARGET = mc

TARGETS = mc test_blink test_governor test_rgb_fade test_32bit_tim

mc.OBJECTS = mc_main.o \
	     exceptions.o \
	     vector_table.o \
	     led.o \
	     usart.o \
	     adc.o \
	     pwm/pwm.o \
	     pwm/pwm_scheme_6step_h_pwm_l_on.o \
	     comm_tim.o \
	     gprot.o \
	     sensor_process.o \
	     comm_process.o \
	     control_process.o

test_blink.OBJECTS = test_blink_main.o \
		   exceptions.o \
		   vector_table.o \
		   led.o

test_governor.OBJECTS = test_governor_main.o \
			exceptions.o \
			vector_table.o \
			led.o \
			usart.o \
			gprot_test_governor.o

test_rgb_fade.OBJECTS = test_rgb_fade_main.o \
			exceptions.o \
			vector_table.o \
			led.o \
			led_fade.o

test_32bit_tim.OBJECTS = test_32bit_tim_main.o \
			 exceptions.o \
			 vector_table.o \
			 led.o \
			 32bit_tim.o \
			 test_32bit_tim_ref_tim.o

OBJECTS = $(test_blink.OBJECTS) $(mc.OBJECTS) $(test_governor.OBJECTS) $(test_rgb_fade.OBJECTS) $(test_32bit_tim.OBJECTS)

.SUFFIXES: .elf .bin .hex .srec .lst
.SECONDEXPANSION:
.SECONDARY:

all: $(DEFAULT_TARGET).all test_blink.all test_governor.all test_rgb_fade.all test_32bit_tim.all

clean: $(DEFAULT_TARGET).clean test_blink.clean test_governor.clean test_rgb_fade.clean test_32bit_tim.clean

flash: $(DEFAULT_TARGET).flash

%.all: $$(*).target_exists $$(*).images $$(*).size
	@echo "*** Finished building $* target ***"

%.images: %.bin %.hex %.srec %.lst
	@echo "*** $* images generated ***"

%.clean: $$(*).target_exists
	@for i in $($(*).OBJECTS) $($(*).OBJECTS:.o=.d) $*.elf $*.hex $*.bin $*.lst $*.srec; do \
		echo "  CLEAN $$i"; \
		rm -f $$i; \
	done

%.flash: $$(*).target_exists $$(*).hex
	@echo "  OOCD  $<"
	$(Q)$(OOCD) -f interface/$(OOCD_INTERFACE).cfg \
		    -f board/$(OOCD_TARGET).cfg \
		    -c init \
		    -c "reset halt" \
		    -c "flash write_image erase $*.hex" \
		    -c reset \
		    -c shutdown

halt:
	@echo " OOCD halt"
	$(Q)$(OOCD) -f interface/$(OOCD_INTERFACE).cfg \
		    -f board/$(OOCD_TARGET).cfg \
		    -c init \
		    -c "reset halt" \
		    -c shutdown

%.size: $$(*).elf
	@echo
	$(Q)$(SIZE) $(SIZEFLAGS) $<

%.elf: $$($$(*).OBJECTS)
	@echo "  LD    $@"
	$(Q)$(LD) $(LDFLAGS) -o $@ $($(*).OBJECTS) $(LDLIBS)

%.target_exists:
	@if [ "_$($(*).OBJECTS)" == "_" ] ; then \
		echo "*** Target $* does not exist ***"; \
		exit 1; \
	fi

# Suffix rules

%.bin: %.elf
	@echo "  CP    $@"
	$(Q)$(CP) $(CPFLAGS) -Obinary $< $@

%.hex: %.elf
	@echo "  CP    $@"
	$(Q)$(CP) $(CPFLAGS) -Oihex $< $@

%.srec: %.elf
	@echo "  CP    $@"
	$(Q)$(CP) $(CPFLAGS) -Osrec $< $@

%.lst: %.elf
	@echo "  OD    $@"
	$(Q)$(OD) $(ODFLAGS) $< > $@

%.o: %.c
	@echo "  CC    $@"
	$(Q)$(CC) $(CFLAGS) -c $*.c -o $*.o

%.d: %.c
	@echo "  DEP   $@"
	$(Q)$(CC) -MM $(CFLAGS) $*.c -MF $*.d
	@cp -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	 sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

# include header dependency information
-include $(OBJECTS:.o=.d))