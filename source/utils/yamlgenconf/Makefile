
ifneq ($(VERBOSE),1)
Q := @
endif

CC=g++
CFLAGS=-Wall -I./include 

ifdef LOG
CFLAGS += -DLOG=$(LOG) -g
endif

LIBS=-lyaml

HDEPS=config.hpp logging.hpp interpreter.hpp register_config.hpp config_generator.hpp \
			error_handling.hpp abstract_config_runner.hpp exception/generator_exception.hpp \
			exception/runner_exception.hpp exception/interpreter_exception.hpp \
			exception/config_exception.hpp exception/parser_exception.hpp \
			register_config_strategy.hpp widget_config.hpp register_config_builder.hpp \
			config_node.hpp flag_config.hpp abstract_register_config_runner.hpp \
			abstract_config_builder.hpp register_config_header_runner.hpp yaml_config.hpp \
			register_group_config.hpp
OBJFILES = yamlgenconf.o yaml_config.o error_handling.o interpreter.o config_node.o \
					 register_config_builder.o register_config_header_runner.o module_config_builder.o \
					 module_config_header_runner.o flag_config_header_runner.o flag_config_builder.o \
					 postprocessor.o

BINDIR=./bin
IDIR=./include
BUILDDIR=./build
SRCDIR=./src

DEPS = $(patsubst %,$(IDIR)/%,$(HDEPS))
OBJ = $(patsubst %,$(BUILDDIR)/%,$(OBJFILES))


$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS) base_dirs
	@echo "  CC    $(@)"
	$(Q)$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

yamlgenconf: $(OBJ) 
	@echo "  LL    $(@)"
	$(Q)$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LIBS)

base_dirs: 
	@mkdir -p $(BUILDDIR) 
	@mkdir -p $(BINDIR) 

.PHONY: clean

clean: 
	$(Q)rm -Rf ./$(BUILDDIR) ./$(BINDIR) ./$(IDIR)/*~ ./$(SRCDIR)/*~ ./*~ 
	@echo "  CLEAN "

