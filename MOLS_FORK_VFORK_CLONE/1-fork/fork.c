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


/**
 * Le FORK duplique l'aspace d'adressage.
 * Donc, le FORK fera les additions de son côté.
 *  Ensuite, le fils sera exit, donc, il n'affichera pas le résultat de ses additions
 *
 * Du côté du père, l'addition ne sera pas faite puisqu'il exécutera seulement le code suivant le "if"
 *
 * Vu que l'espace d'adressage est dupliqué lors du FORK, les variables ne seront modifiées que dans
 *  l'espace d'adressage du FILS.
 *  Donc, les variablses du père ne sont pas modifiées
 */
int main(int argc, char **argv) {

    // Entiers à augmenter dans le fils pour prouver l'espace d'adressage commun
    int a = 5, b = 8;
    // Récupérer la valeur de retour de la fonction créant le proocess fils
    int forkRetNum;
    
    printf("a = %d\n", a);
    printf("b = %d\n", b);
    
    forkRetNum = fork();
    
    if(forkRetNum == 0) { // La création du fils s'est-elle correctement produite ?
        printf("I'm the child !\n");
        // a = 10 but only the one of the chile. not the one of the parent
        a = a + 5;
        printf("Now, a = %d only in the child\n", a);

        // b = 10 but only the one of the chile. not the one of the parent
        b = b + 2;
        printf("Now, b = %d only in the child\n", b);

        printf("PID = %d\n", getpid());
        printf("PPID = %d\n", getppid());
//        printf("Value of vfork is %d.\n", vforkRetNum); // Indiquer la valeur de retour de la fonciton créant le process
        printf("Sum a + b is %d.\n", a + b); // line b
        printf("Let's do a ps to see which process is currenlty running !\n\n");
        printf("CODES D'ÉTAT DE PROCESSUS \nVoici les différentes valeurs que les indicateurs de sortie s, stat et state (en-tête « STAT » ou « S ») afficheront pour décrire l'état d'un processus :\n\n"

               "D    en sommeil non interruptible (normalement entrées et sorties) ;\n"
               "R    s'exécutant ou pouvant s'exécuter (dans la file d'exécution) ;\n"
               "S    en sommeil interruptible (en attente d'un événement pour finir) ;\n"
               "T    arrêté, par un signal de contrôle des tâches ou parce qu'il a été tracé ;\n"
               "W    pagination (non valable depuis le noyau 2.6.xx) ;\n"
               "X    tué (ne devrait jamais être vu) ;\n"
               "Z    processus zombie (<defunct>), terminé mais pas détruit par son parent.\n\n"

       "Pour les formats BSD et quand le mot-clé stat est utilisé, les caractères supplémentaires suivants peuvent être affichés :\n\n"

               "<    haute priorité (non poli pour les autres utilisateurs) ;\n"
               "N    basse priorité (poli pour les autres utilisateurs) ;\n"
               "L    avec ses pages verrouillées en mémoire (pour temps réel et entrées et sorties personnalisées) ;\n"
               "s    meneur de session ;\n"
               "l    possède plusieurs processus légers (« multi-thread », utilisant CLONE_THREAD comme NPTL pthreads le fait) ;\n"
               "+    dans le groupe de processus au premier plan.\n\n");
        while(1){} // Faire en sorte que le fils attende, mais en étant en état d'exécution. Un simple ps le montrera
        exit(0);
    }
    else if (forkRetNum > 0)
    {  // Est-ce le process parent ?
        printf("Ceci est le process parent et le PID est : %d\n", getpid());
    }
    else
    { // Y a-t-il eu une erreur lors de la création du process fils ?
        printf("Problème durant le fork\n");
        exit(EXIT_FAILURE);
    }
    // Parent code
    wait(0); // Pour éviter de faire du fils un zombie
    printf("PID = %d\n", getpid());
    printf("PPID = %d\n", getppid());
    printf("Value of f is %d.\n", forkRetNum); // Indiquer la valeur de retour de la fonciton créant le process
    // La somme est bien de 13 et non plus ni moins puisque la somme fut faite par le fils, mais uniquement avec ses propres variables et non celles du père
    printf("Sum is %d.\n", a + b); // line b
    printf("Let's do a ps to see which process is currenlty running !");
    while(1){} // Simplement pour faire attendre le père que l'on fasse un ps pour pouvoir voir son état
    exit(0);

}
