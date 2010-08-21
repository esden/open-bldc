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

################################################################################
# Please edit the Makefile.targets file when adding targets not this one here
# You will hurt yourself doing that. :)
################################################################################

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


INCDIRS		= \
	-I. \
	-Isrc \
	-Itest \
	-I../libgovernor/include
CFLAGS          = $(INCDIRS) -Wall -ansi -std=c99 -c \
		  -fno-common -Os -g -mcpu=cortex-m3 -mthumb \
		  -ffunction-sections -fdata-sections
ASFLAGS         = -ahls -mapcs-32
LDFLAGS         = -Tsrc/stm32.ld -nostartfiles -L$(TOOLCHAIN_LIB_DIR) -Os \
		                    -L../libgovernor/src/.libs --gc-sections
LDLIBS          = -lcmsis -lstm32 -lgovernor
CPFLAGS         = -j .isr_vector -j .text -j .data
ODFLAGS         = -S
SIZEFLAGS       = -A -x
BUILDDIR	= build/$(TARGET)
BINDIR		= $(BUILDDIR)/bin
OBJDIR		= $(BUILDDIR)/obj
DEPDIR		= build/dep

###############################################################################
# Edit after this point only when you really know what you are doing!!!
###############################################################################

-include Makefile.targets

.SUFFIXES: .elf .bin .hex .srec .lst
.SECONDEXPANSION:
.SECONDARY:

all: $(patsubst %,%.all,$(TARGETS))

clean:
	@echo "Cleaning up everything"
	$(Q)rm -rf build

flash: $(DEFAULT_TARGET).flash

ifdef TARGET
%.all: $$(*).target_exists $(BINDIR)/$$(*).images $(BINDIR)/$$(*).size
	@echo "*** Finished building $* target ***"
else
%.all:
	make TARGET=$(*) $(*).all
endif

$(BINDIR)/%.images: $(BINDIR)/%.bin $(BINDIR)/%.hex $(BINDIR)/%.srec $(BINDIR)/%.lst
	@echo "*** $* images generated ***"


%.clean: $$(*).target_exists
	@echo "Cleaning target $(*)"
	$(Q)rm -rf build/$(*)

ifdef TARGET
%.flash: $$(*).target_exists $$(*).hex
	@echo "  OOCD  $(*).hex"
	$(Q)$(OOCD) -f interface/$(OOCD_INTERFACE).cfg \
		    -f board/$(OOCD_TARGET).cfg \
		    -c init \
		    -c "reset halt" \
		    -c "flash write_image erase $*.hex" \
		    -c reset \
		    -c shutdown
else
%.flash:
	make TARGET=$(*) $(*).flash
endif

halt:
	@echo " OOCD halt"
	$(Q)$(OOCD) -f interface/$(OOCD_INTERFACE).cfg \
		    -f board/$(OOCD_TARGET).cfg \
		    -c init \
		    -c "reset halt" \
		    -c shutdown

.PHONY: doc
doc:
	@mkdir -p doc
	@doxygen doxygen.conf > /dev/null
	@cp ../../art/open-bldc-logo.png doc/doxy/html/

%.size: %.elf
	@echo
	$(Q)$(SIZE) $(SIZEFLAGS) $<

%.elf: $(patsubst %.o,$(OBJDIR)/%.o,$(COMMON_OBJECTS)) $(patsubst %.o,$(OBJDIR)/%.o,$($(TARGET).OBJECTS))
	@echo "  LD    $@"
	$(Q)mkdir -p $(@D)
	$(Q)$(LD) $(LDFLAGS) -o $@ $(COMMON_OBJECTS) $(patsubst %.o,$(OBJDIR)/%.o,$($(TARGET).OBJECTS)) $(LDLIBS)


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

$(OBJDIR)/%.o: %.c
	@echo "  CC    $@"
	$(Q)mkdir -p $(@D)
	$(Q)$(CC) $(CFLAGS) -c $*.c -o $@

$(DEPDIR)/%.d: %.c
	@echo "  DEP   $@"
	$(Q)mkdir -p $(@D)
	$(Q)$(CC) -MM $(CFLAGS) $< -MF $@
	$(Q)cp -f $@ $@.tmp
	$(Q)sed -e 's|.*:|build/$$(TARGET)/obj/$*.o:|' < $@.tmp > $@
	$(Q)sed -e 's/.*://' -e 's/\\$$//' < $@.tmp | fmt -1 | \
	 sed -e 's/^ *//' -e 's/$$/:/' >> $@
	$(Q)rm -f $@.tmp

# include header dependency information
-include $(patsubst %.o,$(DEPDIR)/%.d,$(OBJECTS))
