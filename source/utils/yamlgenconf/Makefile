
CC=g++
CFLAGS=-Wall -I./include -DLOG=$(LOG)
LIBS=-lyaml

HDEPS=yaml_config.hpp logging.hpp interpreter.hpp interpreter_exception.hpp \
			parser_exception.hpp config_node.hpp abstract_generator_strategy.hpp \
			register_config_generator_strategy.hpp generator_exception.hpp \
			register_config_header_runner_strategy.hpp
OBJFILES = yamlgenconf.o yaml_config.o error_handling.o interpreter.o config_node.o \
					 register_config_generator_strategy.o register_config_header_runner_strategy.o

BINDIR=./bin
IDIR=./include
BUILDDIR=./build
SRCDIR=./src

DEPS = $(patsubst %,$(IDIR)/%,$(HDEPS))
OBJ = $(patsubst %,$(BUILDDIR)/%,$(OBJFILES))


$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

yamlgenconf: $(OBJ)
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean: 
	rm -Rf ./$(BUILDDIR)/* ./$(BINDIR)/yamlgenconf ./$(IDIR)/*~ ./$(SRCDIR)/*~ ./*~ 

