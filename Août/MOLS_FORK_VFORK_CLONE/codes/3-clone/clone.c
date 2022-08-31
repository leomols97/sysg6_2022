// Il est nécessaire de définir _GNU_SOURCE pour avoir acces à clone(2) et aux flags CLONE_*

#define _GNU_SOURCE
#include <sched.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sched.h>
#include <linux/sched.h>


/**
 child
 */
static int child_func(void* arg)
{
    char* buffer = (char*)arg;
    printf("Child sees buffer = \"%s\"\n", buffer);
    strcpy(buffer, "Hello from child");
    return 0;
}

//// Cette fonction a pour but de montrer les informations liées à un même process via son PID
//void processStatus(int pid)
//{
//    char parentProcessStatus[18] = "/proc/"; // 18 car si le PID est à 5 chiffres, alors, le tableau de char sera de longueur 18
//
//    char *num;
//    char buff[100];
//
//    // Ce if permet de concaténer un pid_t à un char *
//    if (asprintf(&num, "%d", pid) == -1) {
//        perror("asprintf");
//    } else {
//        strcat(strcpy(buff, parentProcessStatus), num);
//    }
//
//    strcat(buff, "/status");
//    if (fork() == 0)
//	    execl("/bin/cat", "cat", buff, (char *)0);
//}

/**
 Ici, clone() est utilisé de deux manières : une fois avec le flag CLONE_VM (CLONE_VM = clone virtual memory) et une fois sans.
 Un buffer est passé dans le process enfant, et le process enfant y écrit un string.
 Une pile de taille 65536 ensuite allouée pour le process enfant et une fonction qui vérifie si nous exécutons le fichier en utilisant l'option 'vm' (correspondant donc au flag 'CLONE_VM').
 De plus, un buffer de 100 octets est créé dans le process parent et une chaîne y est copiée, puis, l'appel système clone() est exécuté et les erreurs sont vérifiées.

 Lorsqu'une exécution sans l'argument 'vm' se produit, le flag CLONE_VM n'est pas actif et la mémoire virtuelle du process parent est clonée dans le process enfant.
 Le process enfant peut accéder au message passé par le process parent dans le buffer, mais tout ce qui est écrit dans le buffer par l'enfant n'est pas accessible par process parent puisque la mémoire virtuelle est dupliquée pour être allouée au process enfant.
 */
int main(int argc, char** argv)
{
    printf("Selon l'ordonnanceur, les 2 lignes à propos du PID du père et du fils peuvent être écrites plus qu'une fois par ligne.");
    // Alloue un stack pour la tâche du fils
    const int STACK_SIZE = 65536;
    char* stack = malloc(STACK_SIZE);
    if (!stack) { // Si 'stack' n'a pas été correctement créé
        perror("malloc");
        exit(1);
    }

    // Lorsqu'il est appelé avec l'argument 'vm' en ligne de commande, active le flag CLONE_VM.
    unsigned long flags = 0;
    if (argc > 1 && !strcmp(argv[1], "vm")) {

        /**
         int clone(int (*fn)(void *), void *child_stack,
                   int flags, void *arg, ...
                   pid_t *ptid, struct user_desc *tls, pid_t *ctid );
         */

        /**
         Lorsque le process enfant est créé avec clone(), il exécute la fonction fn(arg).
         (Cela diffère de fork(2) dans lequel l'exécution continue dans le fils à partir du point d'appel de fork(2).)
         L'argument fn est un pointeur vers une fonction qui est appelée par le process fils au début de son exécution. L'argument 'arg' est passé à la fonction fn.
         */
        /**
         CLONE_VM (depuis Linux 2.0)
                      Si CLONE_VM est défini, le parent et l'enfant seront exécuté dans le même espace mémoire. En particulier les écritures mémoire effectuées par le parent ou par l'enfant sont également visibles dans l'autre process.
                      De plus, tout mappage ou démappage de mémoire effectué avec mmap(2) ou munmap(2) par le process enfant ou appelant également affecte l'autre process.

                      Si CLONE_VM n'est pas défini, le process enfant s'exécute dans un copie séparée de l'espace mémoire du process appelant au moment de l'appel de clone. Les écritures effectuées par les mappages/démappages par un des process n'affecte pas l'autre, comme avec fork(2).
         */
        flags |= CLONE_VM; // 'flags' vaudra 'CLONE_VM' ou non en fonction du fait que l'option 'vm' soit spécifiée ou non.
    }

    char buffer[100];
    strcpy(buffer, "Hello from parent"); // Ecrit 'hello from parent' dans le buffer
    
    int cloneRetNum;
    // Clone le process père
    // Seul appel à 'clone'. Pour avoir les différentes exécutions, il faut ajouter 'vm' comme argument lors de l'appel en ligne de commande
    // Vu que lorsque CLONE_VM est défini, l'espace d'adressage mémoire est partaé,
    // le buffer est le même pour le père et pour le fils, donc, le fils override ce que le père a écrit par 'Hello from child'
    cloneRetNum = clone(child_func, stack + STACK_SIZE, flags | SIGCHLD, buffer);
    // Selon l'ordonnanceur, les 2 lignes suivantes (à propos du PID du père et du fils) peuvent être écrites plus qu'une fois par ligne.
    printf("PID du fils : %d\n", cloneRetNum);
    printf("PID du père : %d\n", getpid());
//    processStatus(getpid());
//    processStatus(cloneRetNum);
    if (cloneRetNum == -1) {
        perror("clone");
        exit(1);
    }
    
    printf("\n");
    
    int status;
    if (wait(&status) == -1) {
        perror("wait");
        exit(1);
    }

    printf("Child exited with status %d (0 = success). \nbuffer = \"%s\"\n", status, buffer);
    return 0;
}
