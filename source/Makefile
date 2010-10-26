TOPDIR=$(shell pwd)
VARDIR=$(TOPDIR)/var
BUILDDIR=$(VARDIR)/build
STAGEDIR=$(VARDIR)/stage
SOURCEDIR=$(VARDIR)/source
STAGESTM32DIR=$(VARDIR)/stage-stm32
PATCHDIR=$(TOPDIR)/patches

PATCH=patch

.PHONY: all yamlgen olconf libgovernor firmware install clean

all: firmware

#######################################################################
# General util targets
#######################################################################

$(STAGEDIR):
	@mkdir -p $(STAGEDIR)

$(STAGESTM32DIR):
	@mkdir -p $(STAGESTM32DIR)

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

$(SOURCEDIR):
	@mkdir -p $(SOURCEDIR)

#######################################################################
# yamlgen and olconfgen
#######################################################################

olconf-install: $(STAGEDIR) olconf
	+$(MAKE) -C utils/olconf install PREFIX=$(STAGEDIR)

olconf: $(STAGEDIR) yamlgen-install
	+$(MAKE) -C utils/olconf PREFIX=$(STAGEDIR)

yamlgen-install: $(STAGEDIR) yamlgen
	+$(MAKE) -C utils/yamlgen install PREFIX=$(STAGEDIR)

yamlgen: $(STAGEDIR)
	+$(MAKE) -C utils/yamlgen PREFIX=$(STAGEDIR)

#######################################################################
# libgovernor stm32
#######################################################################

libgovernor/configure:
	@cd libgovernor; \
		./autogen.sh

$(BUILDDIR)/libgovernor-stm32/Makefile: libgovernor/configure
	@mkdir -p $(BUILDDIR)/libgovernor-stm32; \
		cd $(BUILDDIR)/libgovernor-stm32; \
		$(TOPDIR)/libgovernor/configure --prefix=$(STAGEDIR) --disable-shared --host=arm-none-eabi --disable-check

libgovernor-stm32: $(BUILDDIR)/libgovernor-stm32/Makefile
	@export PATH=$(STAGEDIR)/bin:$$PATH; \
		$(MAKE) -C $(BUILDDIR)/libgovernor-stm32

libgovernor-stm32-install: libgovernor-stm32
	@export PATH=$(STAGEDIR)/bin:$$PATH; \
		$(MAKE) -C $(BUILDDIR)/libgovernor-stm32 install

#######################################################################
# firmware
#######################################################################

firmware: olconf-install libgovernor-stm32-install
	$(MAKE) -C firmware VERBOSE=$(VERBOSE)

#######################################################################
# QGovernor
#######################################################################

#######################################################################
# Clean
#######################################################################

clean: firmware-clean yamlgen-clean olconf-clean libgovernor-clean libgovernor-stm32-clean

olconf-clean:
	$(MAKE) -C utils/olconf clean

yamlgen-clean:
	$(MAKE) -C utils/yamlgen clean

libgovernor-clean:
	@if [ -f libgovernor/Makefile ]; then \
		$(MAKE) -C libgovernor distclean; \
	fi

libgovernor-stm32-clean:
	@if [ -f $(BUILDDIR)/libgovernor-stm32/Makefile ]; then \
		$(MAKE) -C $(BUILDDIR)/libgovernor-stm32 distclean; \
	fi

firmware-clean:
	$(MAKE) -C firmware clean

qgovernor-clean:
	@if [ -f qgovernor ]; \
		$(MAKE) -C qgovernor clean; \
	fi

