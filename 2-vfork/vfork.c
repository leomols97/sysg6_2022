#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <mm_malloc.h>
#include <spawn.h>
#include <string.h>

/*int main()
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
    if ((errFils = vfork()) == 0)
    {
        a = 20;
        printf ("Je suis le fils\n");
        char * name = "Léo";
        strcpy(buff, *name);
        //printf ("PID = %d\n", getpid());
        //printf ("PPID = %d\n", getppid());
        exit(0); // Remplacer par "wait(0)" pour montrer la différence pour démontrer que seul le fils exécutera la suite puisque c'est le même espace d'adressage, donc, la même TDFO, donc, une fois que la variable sera changée et que le fils se sera occupé de print sur la sortie standard, le père n'aura pus à le faire car stdout ne sera plus dans la TDFO.
        
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
}*/


int main(int argc, char **argv) {

    int a = 5, b = 8;
    int v;

    /**
     * Le VFORK duplique l'aspace d'adressage.
     * Donc, le VFORK fera les additions de son côté, mais dans l'espace d'adressage du PERE
     *  Ensuite, le fils sera exit, donc, il n'affichera pas le résultat de ses additions,
     *  mais les variables sont bien modifiées
     *
     * Du côté du père, l'addition sera pas faite puisqu'il partage l'espace d'adressage du FILS
     *
     * Vu que l'espace d'adressage n'est pas dupliqué lors du VFORK, les variables seront modifiées dans
     *  l'espace d'adressage du PERE (qui est aussi celui du fils)
     *  Donc, les variables du père sont modifiées,
     *   ce qui permet la prise en compte de la modification des valeurs des variables
     */
    v = vfork();
    if(v == 0) {
        // a = 10
        a = a + 5;
        // b = 10
        b = b + 2;
        exit(0);
    }
    // Parent code
    wait(0);
    printf("PID = %d\n", getpid());
    printf("PPID = %d\n", getppid());
    printf("Value of v is %d.\n", v); // line a
    printf("Sum is %d.\n", a + b); // line b
    printf("Let's do a ps to see which process is currenlty running !");
    sleep(4000);
    exit(0);

}
