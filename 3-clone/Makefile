all: default

default: clone

clone: clone.c Makefile
	$(CC) -Wall -Werror -std=gnu11 -ggdb -o clone            clone.c

clean:
	$(RM) clone
