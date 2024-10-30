CC = g++
CFLAGS = -std=c99 -Wall -Wextra -O3 

main: main.o 
	$(CC) $(CFLAGS) -o $@ $< 

main.o: main.cpp

clean:
	rm -f main *.o
