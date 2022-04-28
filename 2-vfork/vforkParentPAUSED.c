#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/**
 vfork() affiche le contenu du 'if{} else{}' deux fois, d'abord dans l'enfant, puis dans le parent.
 Vu que les deux processus partagent le même espace d'adressage, la première sortie contient la valeur du PID correspondant au process fils.
 Dans le bloc if else, le processus enfant est exécuté en premier car il bloque le processus parent lors de son exécution.
 */
int main()
{
    pid_t pid = vfork();  //creating the child process

    printf("Process parent avant le 'if{} else{}': %d\n", getpid());

    if (pid == 0)
    {  // Est-ce le process fils ?
        printf("Ceci est le process fils et le PID est : %d\n\n", getpid());
        exit(0);
    }
    else if (pid > 0)
    {  // Est-ce le process parent ?
        printf("Ceci est le process parent et le PID est : %d\n", getpid());
    }
    else
    { // Y a-t-il eu une erreur lors de la création du process fils ?
        printf("Problème durant le fork\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}
