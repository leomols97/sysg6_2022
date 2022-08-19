all: default

default: vfork vfork1000 vforkParentPAUSED

vfork: vfork.c
	gcc -pthread -o vfork vfork.c
	
vfork1000: vfork1000.c
	gcc -o vfork1000 vfork1000.c -lm
	
vforkParentPAUSED: vforkParentPAUSED.c
	gcc -o vforkParentPAUSED vforkParentPAUSED.c

clean:
	$(RM) vfork vfork1000 vforkParentPAUSED
