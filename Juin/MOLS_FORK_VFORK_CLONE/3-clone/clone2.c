// We have to define the _GNU_SOURCE to get access to clone(2) and the CLONE_*

#define _GNU_SOURCE
#include <sched.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 child
 */
static int child_func(void* arg) {
    char* buffer = (char*)arg;
    printf("Child sees buffer = \"%s\"\n", buffer);
    strcpy(buffer, "hello from child");
    return 0;
}

/**
 Ici, clone() est utilisé de deux manières, une fois avec le drapeau CLONE_VM et une fois sans.
 Un buffer est passé dans le processus enfant, et le processus enfant y écrit une chaîne.
 Une taille de pile est ensuite allouée pour le processus enfant et une fonction qui vérifie si nous exécutons le fichier en utilisant l'option CLONE_VM (vm) est implémentée.
 De plus, un buffer de 100 octets est créé dans le processus parent et une chaîne y est copiée, puis, l'appel système clone() est exécuté et les erreurs sont vérifiées.

 Lorsque d'une exécution sans l'argument vm,
 le drapeau CLONE_VM n'est pas actif et la mémoire virtuelle du processus parent est clonée dans le processus enfant.
 Le processus enfant peut accéder au message passé par le processus parent dans le tampon,
 mais tout ce qui est écrit dans le tampon par l'enfant n'est pas accessible par processus parent.
 */
int main(int argc, char** argv) {
    // Alloue un stack pour la tâche du fils
    const int STACK_SIZE = 65536;
    char* stack = malloc(STACK_SIZE);
    if (!stack) { // Si 'stack' n'a pas été correctement créé
        perror("malloc");
        exit(1);
    }

    // Lorsqu'il est appelé avec l'argument "vm" en ligne de commande, active le flag CLONE_VM.
    unsigned long flags = 0;
    if (argc > 1 && !strcmp(argv[1], "vm")) {

        /**
         int clone(int (*fn)(void *), void *child_stack,
                   int flags, void *arg, ...
                   pid_t *ptid, struct user_desc *tls, pid_t *ctid );
         */

        /**
         Lorsque le processus enfant est créé avec clone(), il exécute la fonction fn(arg).
         (Cela diffère de fork(2), où l'exécution continue dans le fils à partir du point d'appel de fork(2).)
         L'argument fn est un pointeur vers une fonction qui est appelée par le processus fils au début de son exécution. L'argument arg est passé à la fonction fn.
         */
        /**
         CLONE_VM (depuis Linux 2.0)
                      Si CLONE_VM est défini, le parent et l'enfant
                      seront exécuté dans le même espace mémoire. En particulier,
                      les écritures mémoire effectuées par le parent ou par l'
                      enfant sont également visibles dans l'autre processus.
                      De plus, tout mappage ou démappage de mémoire effectué avec
                      mmap(2) ou munmap(2) par le processus enfant ou appelant également
                      affecte l'autre processus.

                      Si CLONE_VM n'est pas défini, le processus enfant s'exécute dans un
                      copie séparée de l'espace mémoire du processus appelant
                      au moment de l'appel de clone. Les écritures effectuées par
                      les mappages/démappages effectués par la mémoire de l'un des processus ne
                      n'affecte pas l'autre, comme avec fork(2).

                      Si l'indicateur CLONE_VM est spécifié et l'indicateur CLONE_VFORK
                      n'est pas spécifié, alors toute autre pile de signaux qui a été
                      établie par sigaltstack(2) est effacée dans l'enfant.
         */
        flags |= CLONE_VM; // 'flags' vaudra 'CLONE_VM' ou non
    }

    char buffer[100];
    strcpy(buffer, "hello from parent"); // Ecrit 'hello from parent' dans le buffer
    // Clone le processus père
    // Seul appel à 'clone'. Pour avoir les différentes exécutions, il faut ajouter 'vm' comme argument lors de l'appel en ligne de commande
    // Vu que lorsque CLONE_VM est défini, l'espace d'adressage mémoiire est partaé,
    // le buffer est le même pour le père et pour le fils, donc, le fils override ce que le père a écrit par 'hello from child'
    if (clone(child_func, stack + STACK_SIZE, flags | SIGCHLD, buffer) == -1) {
        perror("clone");
        exit(1);
    }

    int status;
    if (wait(&status) == -1) {
        perror("wait");
        exit(1);
    }

    printf("Child exited with status %d. buffer = \"%s\"\n", status, buffer);
    return 0;
}
