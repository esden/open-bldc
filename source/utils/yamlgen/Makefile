#
# yamlgen - YAML to everything generator framework
# Copyright (c) 2010 Tobias Fuchs <twh.fuchs@gmail.com>,
#		     Piotr Esden-Tempski <piotr@esden.net>
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

ifneq ($(VERBOSE),1)
Q := @
endif

CC=g++
CFLAGS+=-Wall -I./include -I./include/yamlgen
LDFLAGS+=-lyaml

LIBTOOL=`which libtool > /dev/zero && echo glibtool || echo libtool`

ifdef LOG
CFLAGS += -DLOG=$(LOG) -g
endif

# Objects needed for the test app: 
# BINOBJFILES = error_handling.o interpreter.o config_node.o \
# 							register_config_builder.o register_config_header_runner.o \
# 							module_config_builder.o module_config_header_runner.o \
# 							flag_config_header_runner.o flag_config_builder.o postprocessor.o \
# 							define_config_builder.o define_config_header_runner.o

LIBOBJFILES = interpreter.o config_node.o postprocessor.o

BINDIR=bin
IDIR=include
BUILDDIR=build
SRCDIR=src
DEPDIR=deps
LIBDIR=lib
PREFIX ?= /usr/local

# BINOBJ = $(patsubst %,$(BUILDDIR)/%,$(BINOBJFILES))
LIBOBJ = $(patsubst %.o,$(BUILDDIR)/%.lo,$(LIBOBJFILES))

all: base_dirs $(LIBDIR)/libyamlgen.la
# all: base_dirs yamlgen libyamlgen.so test

$(BUILDDIR)/%.lo: $(SRCDIR)/%.cpp
	@echo "  CC    $(@)"
	$(Q)$(LIBTOOL) --mode=compile --tag=CXX $(CC) $(CFLAGS) -c -o $@ $<

#	yamlgen: $(BINOBJ) 
#		@echo "  LL    $(BINDIR)/$(@)"
#		$(Q)$(CC) -o $(BINDIR)/$(@) $^ $(LDFLAGS)

$(LIBDIR)/libyamlgen.la: $(LIBOBJ)
	@echo "  LL    $(LIBDIR)/$(@)"
	$(Q)$(LIBTOOL) --mode=link --tag=CXX $(CC) $(LDFLAGS) -o $@ $^ -rpath $(PREFIX)/lib

.PHONY: base_dirs
base_dirs: 
	@mkdir -p $(BUILDDIR) 
	@mkdir -p $(BINDIR) 
	@mkdir -p $(LIBDIR) 

.PHONY: clean
clean: 
	$(Q)rm -Rf ./$(BUILDDIR) ./$(BINDIR) ./$(DEPDIR) ./$(LIBDIR) 
	$(Q)find . -name '*~' -exec rm {} \;
	@echo "  CLEAN "

.PHONY: install
install: base_dirs $(LIBDIR)/libyamlgen.la
	@echo "  INST  "
	$(Q)install -d $(PREFIX)/lib
	$(Q)$(LIBTOOL) --mode=install install -c lib/libyamlgen.la $(PREFIX)/lib

$(DEPDIR)/%.d: $(SRCDIR)/%.cpp
	@echo "  DEP   $@"
	$(Q)mkdir -p $(@D)
	$(Q)$(CC) -MM $(CFLAGS) $< -MF $@
	$(Q)cp -f $@ $@.tmp
	$(Q)sed -e 's|.*:|$$(BUILDDIR)/$*.o:|' < $@.tmp > $@
	$(Q)sed -e 's/.*://' -e 's/\\$$//' < $@.tmp | fmt -1 | \
		sed -e 's/^ *//' -e 's/$$/:/' >> $@
	$(Q)rm -f $@.tmp

-include $(patsubst %.o,$(DEPDIR)/%.d,$(BINOBJFILES))
-include $(patsubst %.o,$(DEPDIR)/%.d,$(LIBOBJFILES))
