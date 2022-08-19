all: default

default: clone clone2

clone: clone.c
	gcc -pthread -o clone clone.c

clone2: clone2.c
	gcc -o clone2 clone2.c

clean:
	$(RM) clone clone2
