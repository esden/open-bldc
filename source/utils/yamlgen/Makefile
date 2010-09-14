
ifneq ($(VERBOSE),1)
Q := @
endif

CC=g++
CFLAGS+=-Wall -I./include
LDFLAGS+=-lyaml

ifdef LOG
CFLAGS += -DLOG=$(LOG) -g
endif

OBJFILES = yamlgen.o yaml_config.o error_handling.o interpreter.o \
					 config_node.o register_config_builder.o \
					 register_config_header_runner.o module_config_builder.o \
					 module_config_header_runner.o flag_config_header_runner.o \
					 flag_config_builder.o postprocessor.o define_config_builder.o \
					 define_config_header_runner.o
LIBFILES = libyamlgen.so

BINDIR=./bin
IDIR=./include
BUILDDIR=./build
SRCDIR=./src
DEPDIR=./deps
LIBDIR=./lib

OBJ = $(patsubst %,$(BUILDDIR)/%,$(OBJFILES))
LIB = $(patsubst %,$(LIBDIR)/%,$(LIBFILES))

all: base_dirs yamlgen lib

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "  CC    $(@)"
	$(Q)$(CC) -c -fPIC -o $@ $< $(CFLAGS)

yamlgen: $(OBJ) 
	@echo "  LL    $(@)"
	$(Q)$(CC) -o $(BINDIR)/$@ $^ $(LDFLAGS)

$(LIBDIR)/%.so: $(OBJ)
	@echo "  LL    $(@)"
	$(Q)$(CC) -shared -Wl,-soname,libyamlgen.so.1 -o $@ $^ $(LDFLAGS)

lib: $(LIB)

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

$(DEPDIR)/%.d: $(SRCDIR)/%.cpp
	@echo "  DEP   $@"
	$(Q)mkdir -p $(@D)
	$(Q)$(CC) -MM $(CFLAGS) $< -MF $@
	$(Q)cp -f $@ $@.tmp
	$(Q)sed -e 's|.*:|$$(BUILDDIR)/$*.o:|' < $@.tmp > $@
	$(Q)sed -e 's/.*://' -e 's/\\$$//' < $@.tmp | fmt -1 | \
		sed -e 's/^ *//' -e 's/$$/:/' >> $@
	$(Q)rm -f $@.tmp

-include $(patsubst %.o,$(DEPDIR)/%.d,$(OBJFILES))
