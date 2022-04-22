all: default

default: vfork

vfork: vfork.c Makefile
	gcc -Wall -Werror -std=gnu11 -ggdb -o vfork            vfork.c

clean:
	$(RM) vfork
