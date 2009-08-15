#
# Open-BLDC - Open BrushLess DC Motor Controller
# Copyright (c) 2009 Piotr Esden-Tempski <piotr@esden.net>
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
PREFIX		?= arm-elf
OOCD_INTERFACE	?= jtagkey-tiny
OOCD_TARGET	?= olimex_stm32_h103

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

CFLAGS		= -I. -Ipwm -Wall -ansi -std=c99 -c -fno-common -O0 -g \
		  -mcpu=cortex-m3 -mthumb -ffunction-sections -fdata-sections
ASFLAGS		= -ahls -mapcs-32
LDFLAGS		= -Tstm32.ld -nostartfiles -L$(TOOLCHAIN_LIB_DIR) -Os \
		  --gc-sections
LDLIBS		= -lcmsis -lstm32
CPFLAGS		= -j .isr_vector -j .text -j .data
ODFLAGS		= -S
SIZEFLAGS	= -A -x

OBJECTS = main.o \
	  exceptions.o \
	  vector_table.o \
	  led.o \
	  usart.o \
	  pwm/pwm_utils.o \
	  pwm/pwm.o \
	  pwm/pwm_scheme_6step_h_pwm_l_on.o \
	  pwm/pwm_scheme_6step_h_on_l_pwm.o \
	  pwm/pwm_scheme_6step_on_pwm.o \
	  pwm/pwm_scheme_6step_pwm_on.o \
	  pwm/pwm_scheme_12step_pwm_on_pwm.o \
	  comm_tim.o

all: gccversion images size

.SUFFIXES: .elf .bin .hex .srec .lst

images: $(NAME).bin $(NAME).hex $(NAME).srec $(NAME).lst

$(NAME).elf: $(OBJECTS)
	@echo -e "  LD\t$@"
	$(Q)$(LD) $(LDFLAGS) -o $(NAME).elf $(OBJECTS) $(LDLIBS)

clean:
	@for i in $(OBJECTS) $(NAME).elf $(NAME).hex $(NAME).bin $(NAME).lst $(NAME).srec; do \
		echo -e "  CLEAN\t$$i"; \
		rm -f $$i; \
	done

flash: $(NAME).hex
	@echo -e "  OOCD\t$<"
	$(Q)$(OOCD) -f interface/$(OOCD_INTERFACE).cfg \
		    -f board/$(OOCD_TARGET).cfg \
		    -c init \
		    -c "reset halt" \
		    -c "flash write_image erase $(NAME).hex" \
		    -c reset \
		    -c shutdown

gccversion:
	@$(CC) --version

size: $(NAME).elf
	@echo
	$(Q)$(SIZE) $(SIZEFLAGS) $<

# Suffix rules

.elf.bin:
	@echo -e "  CP\t$@"
	$(Q)$(CP) $(CPFLAGS) -Obinary $< $@

.elf.hex:
	@echo -e "  CP\t$@"
	$(Q)$(CP) $(CPFLAGS) -Oihex $< $@

.elf.srec:
	@echo -e "  CP\t$@"
	$(Q)$(CP) $(CPFLAGS) -Osrec $< $@

.elf.lst:
	@echo -e "  OD\t$@"
	$(Q)$(OD) $(ODFLAGS) $(NAME).elf > $(NAME).lst

%.o: %.c %.h
	@echo -e "  CC\t$@"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@
