
CC=gcc
CFLAGS=-Wall -I./include
LIBS=-lyaml
OBJ=yamlgenconf.o error_handling.o
DEPS=error_handling.h logging.h
BINDIR=./bin
BUILDDIR=./build

%.o: %.c $(DEPS)
	$(CC) -c -o $(BUILDDIR)/$@ $< $(CFLAGS) $(LIBS)

yamlgenconf: $(OBJ)
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean: 
	rm -Rf ./build/* ./*~

