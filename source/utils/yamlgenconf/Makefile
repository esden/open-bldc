
CC=gcc
CFLAGS=-Wall -I.
LIBS=-lyaml
OBJ=yamlgenconfig.o error_handling.o
DEPS=error_handling.h logging.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

yamlgenconfig: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean: 
	rm -f *.o yamlgenconfig *~

