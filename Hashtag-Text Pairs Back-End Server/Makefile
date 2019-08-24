# Usage:
# make				# compile all binary
# make clean		# remove ALL binaries and objects

CC=gcc
CFLAGS=-mno-red-zone

DFLAGS=-DTIED_INSERT_REQUEST=untied
DFLAGS+=-DTIED_INSERT_HASHTABLE=untied
DFLAGS+=-DTIED_QUERY_REQUEST=untied
DFLAGS+=-DTIED_QUERY_HASHTABLE=untied

# DFLAGS+=-DNEW_NODES_INSERTION=0.01
# DFLAGS+=-DPOISSON_ARRIVAL_TIME=0.00001

LD=gcc
LFLAGS=-fopenmp -lm

DEPS=hashtag-text-server.h
OBJ=hashtag-text-server.o 

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) $(DFLAGS) -c -o $@ $<

server: $(OBJ)
	$(LD) -o $@ $^ $(LFLAGS)

.PHONY: clean
clean:
	rm -f *.o *~ server