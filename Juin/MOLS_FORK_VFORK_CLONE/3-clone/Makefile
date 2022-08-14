all: default

default: clone clone2 clone2_optimized

clone: clone.c
	gcc -o clone clone.c

clone2: clone2
	gcc -o clone2 clone2.c

clone2_optimized: clone2.c Makefile
	$(CC) -Wall -Werror -std=gnu17 -Ofast -o clone2_optimized clone2.c

clean:
	$(RM) clone clone2
