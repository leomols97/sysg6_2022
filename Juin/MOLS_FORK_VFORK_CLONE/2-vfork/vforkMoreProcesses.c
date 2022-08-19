#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <mm_malloc.h>
#include <spawn.h>
#include <sys/mman.h>
#include <signal.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>
#include <math.h>



// Pour compiler avec les threads et que les fonctions mathématiques soient reconnues par le compilateur (grâce à l'option '-lm'):
// gcc -pthread -o fork fork.c -lm


// Nombre de duplication qui seront effectuées via 'fork'
int nbVforks = 5;

/**
 Cette fonction permet à l'utilisateur de choisir à quel moment reprendre
 l'exécution du programme pour lui laisser le temps de faire les manipulations qu'il désire
 */
void continueProgram()
{
    printf("Pour continuer le programme, entrez 'continue' ou 'c' : ");
    int c, numberCounter = 0, letterCounter = 0;
    while ((c = getchar()) != 'c')
        if (isalpha(c))
            letterCounter++;
        else if (isdigit(c))
            numberCounter++;
}

/**
 Cette fonction permet de charger le contenu d'un fichier en RAM.

 @param fileName Pour le nom du fichier à lire
 */
void read_file(char fileName[])
{
    FILE* ptr;
    char ch;
    
    // Ouvre le fichier en mode lecture
    ptr = fopen(fileName, "r");
    
    if (NULL == ptr) {
        printf("Le fichier ne peut être ouvert \n");
    }
    
    // Charge ce qui est dans le fichier
    // caractère par caractère
    do {
        ch = fgetc(ptr);
        // Ceci permet d'afficher le contenu du fichier
        //printf("%c", ch);
        
        // Regarde si la fin du fichier n'est pas atteinte
        // Si la fin est atteinte, alors, la lecture s'arrête
    } while (ch != EOF);
    
    // Ferme le fichier
    fclose(ptr);
}

/**
 Crée un fichier text de près de 10Mo
 */
void create10MiBFile()
{
    FILE * fp = fopen("BigFile.txt", "w");
    for (unsigned int i = 0; i < 550000; i++) // 550000 to get to 10Mo with the string to put in
    {
        fprintf(fp, "forkMoreProcesses\n");
    }
    fclose(fp);
}

/**
 * Le FORK duplique l'aspace d'adressage.
 * Donc, le fils fera les additions de son côté, dans son propre espace d'adressage
 *
 * Du côté du père, l'addition ne sera pas faite puisqu'il exécutera seulement le code suivant le "if" qui vérifie le bon code de retour de l'appel à fork()
 *
 * Vu que l'espace d'adressage est dupliqué lors du FORK, les variables ne seront modifiées que dans
 *  l'espace d'adressage du FILS.
 *  Donc, les variablses du père ne sont pas modifiées
 */
int main(int argc, char **argv) {
    
    printf("\n\n\n\nCODES D'ÉTAT DE PROCESSUS \nVoici les différentes valeurs que les indicateurs de sortie s, stat et state (en-tête « STAT » ou « S ») afficheront pour décrire l'état d'un processus :\n\n"
           
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
           "L    les pages du processus sont verrouillées en mémoire;\n"
           "s    meneur de session ;\n"
           "l    possède plusieurs processus légers (« multi-thread », utilisant CLONE_THREAD comme NPTL pthreads le fait) ;\n"
           "+    dans le groupe de processus au premier plan.\n\n\n\n\n\n\n");
    
    
    
    
    
    
    printf("\nDans une autre fenêtre de terminal, entrez la commande 'top' pour voir quels process sont en cours et plsu d'informations, dont leur utilisation de la mémoire et ce, en temps réel !\n\n");
    continueProgram();
    
    create10MiBFile();
    
    // Récupérer la valeur de retour des nbVforks vfork
    int vforkRetNums[nbVforks];
    
    printf("PID du père = %d\n", getpid());
            
    // Nécessaire de faire une suite de vfork en if car, lors d'un vfork
    // Le père est mis en pause et l'espace d'adressage est partagé avec le père
    vforkRetNums[0] = vfork();
    if (vforkRetNums[0] == 0)
    {
        printf("Ceci est le process fils et le PID est : %d\n", getpid());
        read_file("BigFile.txt");
        vforkRetNums[1] = vfork();
        if (vforkRetNums[1] == 0)
        {
            printf("Ceci est le process fils et le PID est : %d\n", getpid());
            read_file("BigFile.txt");
            vforkRetNums[2] = vfork();
            if (vforkRetNums[2] == 0)
            {
                printf("Ceci est le process fils et le PID est : %d\n", getpid());
                read_file("BigFile.txt");
                vforkRetNums[3] = vfork();
                if (vforkRetNums[3] == 0)
                {
                    printf("Ceci est le process fils et le PID est : %d\n", getpid());
                    read_file("BigFile.txt");
                    vforkRetNums[4] = vfork();
                    if (vforkRetNums[4] == 0)
                    {
                        read_file("BigFile.txt");
                        printf("Ceci est le process fils et le PID est : %d\n", getpid());
                        printf("\nVous pouvez observer que, dans le tableau de la commande 'top', une seule ligne concernant ce programme a été créée. Cela signifie que vfork() a créé des process en créant des threads. A ce stade, les fils créés par vfork ne sont que des threads. Ils deviendront des process lorsque les fils fils appelleront une fonction de la famille exec().\n\n");
			printf("\n\nLe fils est en train de tourner à l'infini via un 'while(1)' pour montrer la mémoire utilisée par l'espace des process lié à ce programme (puisque l'espace d'adressage est partagé entre le père et le fils).\n\nPour l'arrêter, dans une autre fenêtre de terminal, entrez la commande 'kill %d' !\n", getpid());
			while(1){} // Faire en sorte que le fils attende, mais en étant en état d'exécution. Un simple 'ps -aux' le montrera
                        exit(0);
                    }
                    else if (vforkRetNums[4] > 0)
                    {  // Est-ce le process parent ?
                        printf("Ceci est le process parent et le PID est : %d\n", getpid());
                    }
                    else
                    { // Y a-t-il eu une erreur lors de la création du process fils ?
                        printf("Problème durant la duplication\n");
                        exit(EXIT_FAILURE);
                    }
                    printf("\n\nLe fils est en train de tourner à l'infini via un 'while(1)' pour montrer la mémoire utilisée par l'espace des process lié à ce programme (puisque l'espace d'adressage est partagé entre le père et le fils).\n\nPour l'arrêter, dans une autre fenêtre de terminal, entrez la commande 'kill %d' !\n", getpid());
    wait((void*)(intptr_t) vforkRetNums[4]); // Eviter de faire du fils un zombie
                    while(1){} // Faire en sorte que le fils attende, mais en étant en état d'exécution. Un simple 'ps -aux' le montrera
                    exit(0);
                }
                else if (vforkRetNums[3] > 0)
                {  // Est-ce le process parent ?
                    printf("Ceci est le process parent et le PID est : %d\n", getpid());
                }
                else
                { // Y a-t-il eu une erreur lors de la création du process fils ?
                    printf("Problème durant la duplication\n");
                    exit(EXIT_FAILURE);
                }
                printf("\n\nLe fils est en train de tourner à l'infini via un 'while(1)' pour montrer la mémoire utilisée par l'espace des process lié à ce programme (puisque l'espace d'adressage est partagé entre le père et le fils).\n\nPour l'arrêter, dans une autre fenêtre de terminal, entrez la commande 'kill %d' !\n", getpid());
    wait((void*)(intptr_t) vforkRetNums[3]); // Eviter de faire du fils un zombie
                while(1){} // Faire en sorte que le fils attende, mais en étant en état d'exécution. Un simple 'ps -aux' le montrera
                exit(0);
            }
            else if (vforkRetNums[2] > 0)
            {  // Est-ce le process parent ?
                printf("Ceci est le process parent et le PID est : %d\n", getpid());
            }
            else
            { // Y a-t-il eu une erreur lors de la création du process fils ?
                printf("Problème durant la duplication\n");
                exit(EXIT_FAILURE);
            }
            printf("\n\nLe fils est en train de tourner à l'infini via un 'while(1)' pour montrer la mémoire utilisée par l'espace des process lié à ce programme (puisque l'espace d'adressage est partagé entre le père et le fils).\n\nPour l'arrêter, dans une autre fenêtre de terminal, entrez la commande 'kill %d' !\n", getpid());
    wait((void*)(intptr_t) vforkRetNums[2]); // Eviter de faire du fils un zombie
            while(1){} // Faire en sorte que le fils attende, mais en étant en état d'exécution. Un simple 'ps -aux' le montrera
            exit(0);
        }
        else if (vforkRetNums[1] > 0)
        {  // Est-ce le process parent ?
            printf("Ceci est le process parent et le PID est : %d\n", getpid());
        }
        else
        { // Y a-t-il eu une erreur lors de la création du process fils ?
            printf("Problème durant la duplication\n");
            exit(EXIT_FAILURE);
        }
        printf("\n\nLe fils est en train de tourner à l'infini via un 'while(1)' pour montrer la mémoire utilisée par l'espace des process lié à ce programme (puisque l'espace d'adressage est partagé entre le père et le fils).\n\nPour l'arrêter, dans une autre fenêtre de terminal, entrez la commande 'kill %d' !\n", getpid());
    wait((void*)(intptr_t) vforkRetNums[1]); // Eviter de faire du fils un zombie
        while(1){} // Faire en sorte que le fils attende, mais en étant en état d'exécution. Un simple 'ps -aux' le montrera
        exit(0);
    }
    else if (vforkRetNums[0] > 0)
    {  // Est-ce le process parent ?
        printf("Ceci est le process parent et le PID est : %d\n", getpid());
    }
    else
    { // Y a-t-il eu une erreur lors de la création du process fils ?
        printf("Problème durant la duplication\n");
        exit(EXIT_FAILURE);
    }
    
    // Code du père
    wait((void*)(intptr_t) vforkRetNums[0]); // Eviter de faire du fils un zombie
    printf("Les fils sont terminés\n");
    printf("\n\nAu fur et à mesure que vous killiez les process un à un, vous avez pu observer que la mémoire occupée par le process courant n'a pas diminué, ce qui prouve que l'espace d'adressage est partagé entre un process père et un process fils si le moyen de duplication de process est 'vfork()' !\n");
    printf("\n\nPID (du père, donc) = %d\n", getpid());
    printf("\nPPID (id du process à l'origine de la création du programme) = %d\n", getppid());
    
    printf("\nDans une autre fenêtre de terminal, entrez la commande 'ps -aux' pour voir quel process est en cours et plus d'informations à leurs propos !\n\n");
    
    printf ("\n\nLe programme ne se termine pas pour laisser le temps de faire un 'ps -aux' et voir quels process sont en cours d'exécution et leurs états. Pour le terminer, faites un 'kill %d' dans une autre fenêtre de terminal ou faites un CTRL+C ici. Vous verrez alors dans l'affichage généré par la commande 'top' que la mémoire occupée par le process courant est libérée\n", getpid());
    
    while(1){} // Simplement pour faire attendre le père. Un simple 'ps -aux' montrera son état
    
    exit(0);
}


CODES D'ÉTAT DE PROCESSUS 
Voici les différentes valeurs que les indicateurs de sortie s, stat et state (en-tête « STAT » ou « S ») afficheront pour décrire l'état d'un processus :

D    en sommeil non interruptible (normalement entrées et sorties) ;
R    s'exécutant ou pouvant s'exécuter (dans la file d'exécution) ;
S    en sommeil interruptible (en attente d'un événement pour finir) ;
T    arrêté, par un signal de contrôle des tâches ou parce qu'il a été tracé ;
W    pagination (non valable depuis le noyau 2.6.xx) ;
X    tué (ne devrait jamais être vu) ;
Z    processus zombie (<defunct>), terminé mais pas détruit par son parent.

Pour les formats BSD et quand le mot-clé stat est utilisé, les caractères supplémentaires suivants peuvent être affichés :

<    haute priorité (non poli pour les autres utilisateurs) ;
N    basse priorité (poli pour les autres utilisateurs) ;
L    les pages du processus sont verrouillées en mémoire;
s    meneur de session ;
l    possède plusieurs processus légers (« multi-thread », utilisant CLONE_THREAD comme NPTL pthreads le fait) ;
+    dans le groupe de processus au premier plan.







Dans une autre fenêtre de terminal, entrez la commande 'top' pour voir quels process sont en cours et plsu d'informations, dont leur utilisation de la mémoire et ce, en temps réel !

Pour continuer le programme, entrez 'continue' ou 'c' : c
PID du père = 98674
Ceci est le process fils et le PID est : 98675
Ceci est le process fils et le PID est : 98676
Ceci est le process fils et le PID est : 98678
Ceci est le process fils et le PID est : 98679
Ceci est le process fils et le PID est : 98680

Vous pouvez observer que, dans le tableau de la commande 'top', une seule ligne concernant ce programme a été créée. Cela signifie que vfork() a créé des process en créant des threads. A ce stade, les fils créés par vfork ne sont que des threads. Ils deviendront des process lorsque les fils fils appelleront une fonction de la famille exec().



Le fils est en train de tourner à l'infini via un 'while(1)' pour montrer la mémoire utilisée par l'espace des process lié à ce programme (puisque l'espace d'adressage est partagé entre le père et le fils).

Pour l'arrêter, dans une autre fenêtre de terminal, entrez la commande 'kill 98680' !
Ceci est le process parent et le PID est : 98679


Le fils est en train de tourner à l'infini via un 'while(1)' pour montrer la mémoire utilisée par l'espace des process lié à ce programme (puisque l'espace d'adressage est partagé entre le père et le fils).

Pour l'arrêter, dans une autre fenêtre de terminal, entrez la commande 'kill 98679' !
Ceci est le process parent et le PID est : 98678


Le fils est en train de tourner à l'infini via un 'while(1)' pour montrer la mémoire utilisée par l'espace des process lié à ce programme (puisque l'espace d'adressage est partagé entre le père et le fils).

Pour l'arrêter, dans une autre fenêtre de terminal, entrez la commande 'kill 98678' !
Ceci est le process parent et le PID est : 98676


Le fils est en train de tourner à l'infini via un 'while(1)' pour montrer la mémoire utilisée par l'espace des process lié à ce programme (puisque l'espace d'adressage est partagé entre le père et le fils).

Pour l'arrêter, dans une autre fenêtre de terminal, entrez la commande 'kill 98676' !
Ceci est le process parent et le PID est : 98675


Le fils est en train de tourner à l'infini via un 'while(1)' pour montrer la mémoire utilisée par l'espace des process lié à ce programme (puisque l'espace d'adressage est partagé entre le père et le fils).

Pour l'arrêter, dans une autre fenêtre de terminal, entrez la commande 'kill 98675' !
Ceci est le process parent et le PID est : 98674
Les fils sont terminés


Au fur et à mesure que vous killiez les process un à un, vous avez pu observer que la mémoire occupée par le process courant n'a pas diminué, ce qui prouve que l'espace d'adressage est partagé entre un process père et un process fils si le moyen de duplication de process est 'vfork()' !


PID (du père, donc) = 98674

PPID (id du process à l'origine de la création du programme) = 3126

Dans une autre fenêtre de terminal, entrez la commande 'ps -aux' pour voir quel process est en cours et plus d'informations à leurs propos !



Le programme ne se termine pas pour laisser le temps de faire un 'ps -aux' et voir quels process sont en cours d'exécution et leurs états. Pour le terminer, faites un 'kill 98674' dans une autre fenêtre de terminal ou faites un CTRL+C ici. Vous verrez alors dans l'affichage généré par la commande 'top' que la mémoire occupée par le process courant est libérée
Terminated

