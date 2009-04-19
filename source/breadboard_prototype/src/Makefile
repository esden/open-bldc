##
# Open-BLDC - Open BruschLess DC Motor Controller
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
##

NAME   ?= open-bldc-bb

PREFIX ?= arm-none-eabi

CC      = $(PREFIX)-gcc
LD      = $(PREFIX)-ld
AR      = $(PREFIX)-ar
AS      = $(PREFIX)-as
CP      = $(PREFIX)-objcopy
OD	= $(PREFIX)-objdump

TOOLCHAIN_LIB_DIR = `dirname \`which $(CC)\``/../$(PREFIX)/lib

CFLAGS  =  -I. -Wall -ansi -std=c99 -c -fno-common -Os -g -mcpu=cortex-m3 -mthumb
ASFLAGS = -ahls -mapcs-32
LDFLAGS = -Tstm32.ld -nostartfiles -L$(TOOLCHAIN_LIB_DIR)
LDLIBS  = -lstm32
ODFLAGS	= -S

OBJECTS = main.o interrupts.o vector_table.o

all: images

.SUFFIXES: .elf .bin .hex .srec .list

images: $(NAME).bin $(NAME).hex $(NAME).srec $(NAME).list


$(NAME).elf: $(OBJECTS)
	@echo -e "  LD\t$@"
	@$(LD) $(LDFLAGS) -o $(NAME).elf $(OBJECTS) $(LDLIBS)

clean:
	@for i in $(OBJECTS) $(NAME).elf $(NAME).hex $(NAME).bin $(NAME).list $(NAME).srec; \
	do \
		echo -e "  CLEAN\t$$i"; \
		rm -f $$i; \
	done

flash: $(NAME).hex
	@echo -e "  OOCD\t$<"
	@openocd -f interface/jtagkey-tiny.cfg \
		 -f board/olimex_stm32_h103.cfg \
		 -c init \
		 -c "reset halt" \
		 -c "flash write_image erase open-bldc-bb.hex" \
		 -c reset \
		 -c shutdown

# Suffix rules

.elf.bin:
	@echo -e "  CP\t$@"
	@$(CP) -Obinary $< $@

.elf.hex:
	@echo -e "  CP\t$@"
	@$(CP) -Oihex $< $@

.elf.srec:
	@echo -e "  CP\t$@"
	@$(CP) -Osrec $< $@

.elf.list:
	@echo -e "  OD\t$@"
	@$(OD) $(ODFLAGS) $(NAME).elf > $(NAME).list

.c.o:
	@echo -e "  CC\t$@"
	@$(CC) $(CFLAGS) -c $<
