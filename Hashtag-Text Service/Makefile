# Usage:
# make				# compile all binary
# make clean		# remove ALL binaries and objects

CC=gcc
CFLAGS=-mno-red-zone

DFLAGS=-DTIED_INSERT_REQUEST=untied
DFLAGS+=-DTIED_INSERT_HASHTABLE=untied
DFLAGS+=-DTIED_QUERY_REQUEST=untied
DFLAGS+=-DTIED_QUERY_HASHTABLE=untied

DFLAGS+=-DMANUAL_CUT_OFF=0

# DFLAGS+=-DNEW_NODES_INSERTION=0.01
# DFLAGS+=-DPOISSON_ARRIVAL_TIME=0.00001

LD=gcc
LFLAGS=-fopenmp -lm

DEPS=hashtag-text-service.h
OBJ=hashtag-text-service.o 

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) $(DFLAGS) -c -o $@ $<

service: $(OBJ)
	$(LD) -o $@ $^ $(LFLAGS)

.PHONY: clean
clean:
	rm -f *.o *~ service