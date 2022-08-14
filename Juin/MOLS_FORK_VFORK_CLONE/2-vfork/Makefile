all: default

default: vfork vforkParentPAUSED

vfork: vfork.c
	gcc -pthread -o vfork vfork.c
	
vforkParentPAUSED: vforkParentPAUSED.c
	gcc -o vforkParentPAUSED vforkParentPAUSED.c

clean:
	$(RM) vfork
	$(RM) vforkParentPAUSED
