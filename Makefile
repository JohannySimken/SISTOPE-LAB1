CC = gcc
CFLAGS = -Wall -Wextra

all: lab1

lab1: lab1.c funciones.c funciones.h
	$(CC) $(CFLAGS) -o lab1 lab1.c funciones.c -lm

clean:
	rm -f lab1