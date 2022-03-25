#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <mm_malloc.h>
#include <spawn.h>
#include <string.h>

int main()
{
    int a = 10;
    pid_t errFils = 0;
    int * adress = malloc(sizeof(int));
    char buff[10];
    printf ("\nBefore Forking\n");
    printf ("Je suis le père\n");
    printf ("a = %d\n", a);
    printf ("Adress of malloc : %d\n", adress);
    printf ("Valeur de retour du vfork : %d\n", errFils);
    printf ("PID = %d\n", getpid());
    printf ("PPID = %d\n\n", getppid());
    if ((errFils = fork()) == 0)
    {
        a = 20;
        printf ("Je suis le fils\n");
        char * name = "Léo";
        strcpy(buff, *name);
        //printf ("PID = %d\n", getpid());
        //printf ("PPID = %d\n", getppid());
        //exit(0); // Remplacer par "wait(0)" pour montrer la différence pour démontrer que seul le fils exécutera la suite puisque c'est le même espace d'adressage, donc, la même TDFO, donc, une fois que la variable sera changée et que le fils se sera occupé de print sur la sortie standard, le père n'aura pus à le faire car stdout ne sera plus dans la TDFO.
        
        // wait est un processus bloquant. Donc, la suite ne sera pas exécutée tant qu'une condition ne sera pas remplie. Si l'on met un pointeur d'un nombre, alors, on pourra récupérer le code de terminaison du processus enfant. Pareil pour exit
        
        // Attention : le "exit(0)" le tue, mais ne l'enlève pas de la table des process et envoie un signal à son parent
        
        // Si l'on met cette ligne en commentaire (celle avec le "exit(0)" ou le "wait(0)"), alors, une erreur de segmentation sera renvoyée
    }
    //wait(errFils);
    printf ("After Forking\n");
    printf ("a = %d\n", a);
    printf ("Adress of malloc : %d\n", adress);
    printf ("Valeur de retour du vfork : %d\n", errFils);
    printf ("PID = %d\n", getpid());
    printf ("PPID = %d\n\n", getppid());
}
