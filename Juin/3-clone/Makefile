all: default

default: clone2 clone2_optimized

clone2: clone2 Makefile
	$(CC) -Wall -Werror -std=gnu17 -ggdb -o clone2            clone2.c
  
clone2_optimized: clone2.c Makefile
	$(CC) -Wall -Werror -std=gnu17 -Ofast -o clone2_optimized clone2.c

clean:
	$(RM) waitpid waitpid_optimized
