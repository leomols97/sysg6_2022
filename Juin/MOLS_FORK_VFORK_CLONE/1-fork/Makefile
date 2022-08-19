all: default

default: fork fork1000

fork: fork.c
	gcc -pthread -o fork fork.c
	
fork1000: fork1000.c
	gcc -pthread -o fork fork.c -lm
	
clean:
	$(RM) fork
