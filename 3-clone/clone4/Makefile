all: default

default: waitpid

waitpid: waitpid.c Makefile
	$(CC) -Wall -Werror -std=gnu11 -ggdb -o waitpid            waitpid.c

clean:
	$(RM) waitpid
