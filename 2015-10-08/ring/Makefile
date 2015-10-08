CC=mpicc
CFLAGS=-std=gnu99 -Wall

.PHONY: clean

all: ring.x ring-nb.x

%.x: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f ring.x ring-nb.x ring-*.o*

