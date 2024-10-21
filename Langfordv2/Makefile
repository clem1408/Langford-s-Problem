CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -O3 

main: main.o langford.o
	$(CC) $(CFLAGS) -o $@ $< langford.o

main.o: main.c langford.h

clean:
	rm -f main *.o
