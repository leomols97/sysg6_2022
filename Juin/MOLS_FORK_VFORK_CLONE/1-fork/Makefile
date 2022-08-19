all: default

default: fork forkMoreProcesses

fork: fork.c
	gcc -pthread -o fork fork.c
	
forkMoreProcesses: forkMoreProcesses.c
	gcc -o forkMoreProcesses forkMoreProcesses.c -lm
	
clean:
	$(RM) fork forkMoreProcesses
