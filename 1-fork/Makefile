all: default

default: fork

fork: fork.c Makefile
	$(CC) -Wall -Werror -std=gnu11 -ggdb -o fork            fork.c

clean:
	$(RM) fork
