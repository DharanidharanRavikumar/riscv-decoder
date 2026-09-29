CC = gcc
CFLAGS = -Wall -Wextra -std=c11

decoder: main.c decoder.c decoder.h 
	$(CC) $(CFLAGS) main.c decoder.c decoder.h -o decoder

clean:
	rm -f decoder
	