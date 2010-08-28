
CC=g++
CFLAGS=-Wall -I./include -DLOG=$(LOG)
LIBS=-lyaml

HDEPS=error_handling.hpp logging.hpp interpreter.hpp
OBJFILES = yamlgenconf.o error_handling.o interpreter.o

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

