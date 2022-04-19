all: default

default: waitpid waitpid_optimized

waitpid: waitpid.c Makefile
	$(CC) -Wall -Werror -std=gnu11 -ggdb -o waitpid            waitpid.c
  
waitpid_optimized: waitpid.c Makefile
	$(CC) -Wall -Werror -std=gnu11 -Ofast -o waitpid_optimized waitpid.c

clean:
	$(RM) waitpid waitpid_optimized
