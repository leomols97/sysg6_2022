#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <mm_malloc.h>
#include <spawn.h>

/*int main()
{
    int a = 10, errFils;
    printf ("Before Forking\n");
    printf ("%d\n", a);
    if ((errFils = fork()) == 0)
    {
        a = 20;
        //exit(0); // Remplacer par "wait(0)" pour montrer la différence pour démontrer que seul le fils exécutera la suite puisque c'est le même espace d'adressage, donc, la même TDFO, donc, une fois que la variable sera changée et que le fils se sera occupé de print sur la sortie standard, le père n'aura pus à le faire car stdout ne sera plus dans la TDFO.
        
        // wait est un processus bloquant. Donc, la suite ne sera pas exécutée tant qu'une condition ne sera pas remplie. Si l'on met un pointeur d'un nombre, alors, on pourra récupérer le code de terminaison du processus enfant. Pareil pour exit
        
        // Attention : le "exit(0)" le tue, mais ne l'enlève pas de la table des process et envoie un signal à son parent
    }
    wait(errFils);
    printf ("After Forking\n");
    printf ("%d\n", a);
}*/

int main(int argc, char **argv) {

    int a = 5, b = 8;
    int v;

    /**
     * Le FORK duplique l'aspace d'adressage.
     * Donc, le FORK fera les additions de son côté.
     *  Ensuite, le fils sera exit, donc, il n'affichera pas le résultat de ses additions
     *
     * Du côté du père, l'addition ne sera pas faite puisqu'il exécutera seulement le code suivant le "if"
     *
     * Vu que l'espace d'adressage est dupliqué lors du FORK, les variables ne seront modifiées que dans
     *  l'espace d'adressage du FILS.
     *  Donc, les variables du père ne sont pas modifiées
     */
    v = fork();
    if(v == 0) {
        // 10
        a = a + 5;
        // 10
        b = b + 2;
        exit(0);
    }
    // Parent code
    wait(0);
    printf("PID = %d\n", getpid());
    printf("PPID = %d\n", getppid());
    printf("Value of v is %d.\n", v); // line a
    printf("Sum is %d.\n", a + b); // line b
    exit(0);

}
