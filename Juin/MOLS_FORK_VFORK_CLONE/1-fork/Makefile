all: default

default: fork fork1000

fork: fork.c
	gcc -pthread -o fork fork.c
	
fork1000: fork1000.c
	gcc -o fork1000 fork1000.c -lm
	
clean:
	$(RM) fork fork1000
