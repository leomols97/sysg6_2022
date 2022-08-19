all: default

default: vfork vforkParentPAUSED vforkMoreProcesses

vfork: vfork.c
	gcc -pthread -o vfork vfork.c
		
vforkParentPAUSED: vforkParentPAUSED.c
	gcc -o vforkParentPAUSED vforkParentPAUSED.c
	
vforkMoreProcesses: vforkMoreProcesses.c
	gcc -o vforkMoreProcesses vforkMoreProcesses.c -lm

clean:
	$(RM) vfork vforkParentPAUSED vforkMoreProcesses
