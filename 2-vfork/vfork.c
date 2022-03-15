#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <mm_malloc.h>
#include <spawn.h>

int main()
{
    int a = 10, errFils;
    //int * adress = malloc(sizeof(int));
    //*adress = 10;
    printf ("Before Forking\n");
    printf ("%d\n", a);
    //printf ("%d\n", adress);
    if ((errFils = vfork()) == 0)
    {
        a = 20;
        exit(0); // Remplacer par "wait(0)" pour montrer la différence pour démontrer que seul le fils exécutera la suite puisque c'est le même espace d'adressage, donc, la même TDFO, donc, une fois que la variable sera changée et que le fils se sera occupé de print sur la sortie standard, le père n'aura pus à le faire car stdout ne sera plus dans la TDFO.
        
        // wait est un processus bloquant. Donc, la suite ne sera pas exécutée tant qu'une condition ne sera pas remplie. Si l'on met un pointeur d'un nombre, alors, on pourra récupérer le code de terminaison du processus enfant. Pareil pour exit
        
        // Attention : le "exit(0)" le tue, mais ne l'enlève pas de la table des process et envoie un signal à son parent
        
        // Si l'on met cette ligne en commentaire (celle avec le "exit(0)" ou le "wait(0)"), alors, une erreur de segmentation sera renvoyée
    }
    //wait(errFils);
    //printf ("%d", getpid());
    printf ("After Forking\n");
    printf ("%d\n", a);
    //printf ("%d\n", adress);
}
