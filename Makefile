

.PHONY: all yamlgen olconf libgovernor firmware install clean

all: yamlgen olconf libgovernor firmware

olconf:
	+$(MAKE) -C source/utils/olconf

yamlgen:
	+$(MAKE) -C source/utils/yamlgen

libgovernor:
	+$(MAKE) -C source/libgovernor

firmware:
	+$(MAKE) -C source/firmware

install: olconf-install yamlgen-install libgovernor-install firmware-install

olconf-install:
	+$(MAKE) -C source/utils/olconf install

yamlgen-install:
	+$(MAKE) -C source/utils/yamlgen install

libgovernor-install:
	+$(MAKE) -C source/libgovernor install

firmware-install:
	+$(MAKE) -C source/firmware install

clean: firmware-clean libgovernor-clean yamlgen-clean olconf-clean

olconf-clean:
	$(MAKE) -C source/utils/olconf clean

yamlgen-clean:
	$(MAKE) -C source/utils/yamlgen clean

libgovernor-clean:
	$(MAKE) -C source/libgovernor clean

firmware-clean:
	$(MAKE) -C source/firmware clean



