

.PHONY: all yamlgen olconf libgovernor firmware install clean

all: yamlgen olconf libgovernor firmware

olconf:
	+$(MAKE) -C utils/olconf

yamlgen:
	+$(MAKE) -C utils/yamlgen

libgovernor:
	+$(MAKE) -C libgovernor

firmware:
	+$(MAKE) -C firmware

install: olconf-install yamlgen-install libgovernor-install firmware-install

olconf-install:
	+$(MAKE) -C utils/olconf install

yamlgen-install:
	+$(MAKE) -C utils/yamlgen install

libgovernor-install:
	+$(MAKE) -C libgovernor install

firmware-install:
	+$(MAKE) -C firmware install

clean: firmware-clean libgovernor-clean yamlgen-clean olconf-clean

olconf-clean:
	$(MAKE) -C utils/olconf clean

yamlgen-clean:
	$(MAKE) -C utils/yamlgen clean

libgovernor-clean:
	$(MAKE) -C libgovernor clean

firmware-clean:
	$(MAKE) -C firmware clean



