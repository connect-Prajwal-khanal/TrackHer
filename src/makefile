# Makefile
CC = gcc
CFLAGS = -Wall -g
OBJ = main.o backend.o

all: src

src: $(OBJ)
	$(CC) $(CFLAGS) -o src $(OBJ) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o src