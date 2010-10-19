TOPDIR=`pwd`
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
# summon-arm-toolchain
#######################################################################

$(SOURCEDIR)/summon-arm-toolchain: $(SOURCEDIR)
	@cd $(SOURCEDIR); \
		wget -c http://github.com/esden/summon-arm-toolchain/raw/master/summon-arm-toolchain

$(BUILDDIR)/sat/summon-arm-toolchain: $(SOURCEDIR)/summon-arm-toolchain
	@mkdir -p $(BUILDDIR)/sat; \
		$(PATCH) -p0 -i $(PATCHDIR)/summon-arm-toolchain.patch -o $(BUILDDIR)/sat/summon-arm-toolchain-template; \
		cat $(BUILDDIR)/sat/summon-arm-toolchain-template | sed s,stagedirectory,$(STAGEDIR), > $(BUILDDIR)/sat/summon-arm-toolchain


sat: $(BUILDDIR)/sat/summon-arm-toolchain
	@cd $(BUILDDIR)/sat; \
		sh ./summon-arm-toolchain

#######################################################################
# libgovernor stm32
#######################################################################

libgovernor/configure:
	@cd libgovernor; \
		./autogen.sh

libgovernor-stm32-configure: libgovernor/configure sat
	@mkdir -p $(BUILDDIR)/libgovernor-stm32; \
		export PATH=$(STAGEDIR)/bin:$$PATH; \
		export TOPDIR=$(TOPDIR); \
		export STAGEDIR=$(STAGEDIR); \
		cd $(BUILDDIR)/libgovernor-stm32; \
		$$TOPDIR/libgovernor/configure --prefix=$$STAGEDIR --disable-shared --host=arm-none-eabi --disable-check

libgovernor-stm32: libgovernor-stm32-configure
	@export PATH=$(STAGEDIR)/bin:$$PATH; \
		$(MAKE) -C $(BUILDDIR)/libgovernor-stm32

libgovernor-stm32-install: libgovernor-stm32
	@export PATH=$(STAGEDIR)/bin:$$PATH; \
		$(MAKE) -C $(BUILDDIR)/libgovernor-stm32 install

#######################################################################
# firmware
#######################################################################

firmware: olconf-install libgovernor-stm32-install
	@export PATH=$(STAGEDIR)/bin:$$PATH; \
		export CFLAGS="-I$(STAGEDIR)/include"; \
		export LDFLAGS="-L$(STAGEDIR)/lib"; \
		echo $$PATH; \
		echo `which arm-none-eabi-gcc`; \
		$(MAKE) -C firmware

#######################################################################
# QGovernor
#######################################################################

#######################################################################
# Clean
#######################################################################

clean: firmware-clean libgovernor-clean yamlgen-clean olconf-clean

olconf-clean:
	$(MAKE) -C utils/olconf clean

yamlgen-clean:
	$(MAKE) -C utils/yamlgen clean

libgovernor-clean:
	$(MAKE) -C libgovernor distclean

firmware-clean:
	$(MAKE) -C firmware clean

qgovernor-clean:
	$(MAKE) -C qgovernor clean

