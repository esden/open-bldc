
ifneq ($(VERBOSE),1)
Q := @
endif

CC=g++
CFLAGS+=-Wall -I./include
LDFLAGS+=-lyaml

ifdef LOG
CFLAGS += -DLOG=$(LOG) -g
endif

# Objects needed for the console app:
BINOBJFILES = olconfgen.o yaml_config.o 

# Objects needed for the shared library: 
LIBOBJFILES = error_handling.o interpreter.o config_node.o \
							register_config_builder.o register_config_header_runner.o \
							module_config_builder.o module_config_header_runner.o \
							flag_config_header_runner.o flag_config_builder.o \
							postprocessor.o define_config_builder.o \
							define_config_header_runner.o

BINDIR=bin
IDIR=include
BUILDDIR=build
SRCDIR=src
DEPDIR=deps
LIBDIR=lib

BINOBJ = $(patsubst %,$(BUILDDIR)/%,$(BINOBJFILES))
LIBOBJ = $(patsubst %,$(BUILDDIR)/%,$(LIBOBJFILES))

all: base_dirs libolconf.so olconfgen test

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "  CC    $(@)"
	$(Q)$(CC) -c -fPIC -o $@ $< $(CFLAGS)

olconfgen: $(BINOBJ) 
	@echo "  LL    $(BINDIR)/$(@)"
	$(Q)$(CC) -o $(BINDIR)/$(@) $^ $(LDFLAGS)

libolconf.so: $(LIBOBJ)
	@echo "  LL    $(LIBDIR)/$(@)"
	$(Q)$(CC) -shared -Wl,-soname,libolconf -o $(LIBDIR)/$(@) $^ $(LDFLAGS)

.PHONY: base_dirs
base_dirs: 
	@mkdir -p $(BUILDDIR) 
	@mkdir -p $(BINDIR) 
	@mkdir -p $(LIBDIR) 

.PHONY: clean
clean: 
	$(Q)rm -Rf ./$(BUILDDIR) ./$(BINDIR) ./$(DEPDIR) ./$(LIBDIR) 
	$(Q)find -name '*~' -exec rm {} \;
	@echo "  CLEAN "

.PHONY: test
test: yamlgen
	@echo "  TEST  "
	$(Q)./bin/olconfgen ./test/main.yaml >test/test_output.hpp
	$(Q)diff test/test_output.hpp test/expected_main.hpp
	$(Q)rm test/test_output.hpp

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
