
CC=g++
CFLAGS=-Wall -I./include -DLOG=$(LOG)
LIBS=-lyaml

HDEPS=yaml_config.hpp logging.hpp yaml_interpreter.hpp yaml_interpreter_exception.hpp \
			parser_exception.hpp config_node.hpp abstract_yaml_generator_strategy.hpp \
			yaml_generator_register_config_strategy.hpp yaml_generator_exception.hpp
OBJFILES = yamlgenconf.o yaml_config.o error_handling.o yaml_interpreter.o config_node.o \
					 yaml_generator_register_config_strategy.o

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

