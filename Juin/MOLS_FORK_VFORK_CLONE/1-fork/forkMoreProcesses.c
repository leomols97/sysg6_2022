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
#include <sys/stat.h>
#include <fcntl.h>



// Pour compiler avec les threads et que les fonctions mathématiques soient reconnues par le compilateur (grâce à l'option '-lm'):
// gcc -pthread -o fork fork.c -lm


// Nombre de duplication qui seront effectuées via 'fork'
int nbForks = 5;

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
 Crée un fichier text de près de 10Mo
 */
void create10MiBFile()
{
    FILE * fp=fopen("BigFile.txt", "w");
    for (unsigned int i = 0; i < 550000; i++) // 550000 to get to 10Mo with the string to put in
    {
        fprintf(fp, "forkMoreProcesses\n");
    }
    fclose(fp);
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
    
    
    
    printf("\nDans une autre fenêtre de terminal, entrez la commande 'top' pour voir quels process sont en cours et plsu d'informations, dont leur utilisation de la mémoire et ce, en temps réel !\n Ceci permettra d'observer que 5 lignes seront créées dans le tableau du résultat de la commande car fork() crée des process à part entière.\n\n");
    continueProgram();
    
    create10MiBFile();
    
    // Récupérer la valeur de retour des nbForks fork dans un tableau
    int forkRetNums[nbForks];
    
    printf("PID du père = %d\n", getpid());
    for (unsigned int i = 0; i < nbForks; i++)
    {
        read_file("BigFile.txt");
        forkRetNums[i] = fork();
        
        if(forkRetNums[i] == 0)
        {  // Est-ce le process fils ?
            printf("Ceci est le process fils et le PID est : %d\n", getpid());
            
            //continueProgram();
            while(1){} // Faire en sorte que le fils attende, mais en étant en état d'exécution. Un simple 'ps -aux' le montrera
            exit(0);
        }
        else if (forkRetNums[i] > 0)
        {  // Est-ce le process parent ?
            printf("Ceci est le process parent et le PID est : %d\n", getpid());
        }
        else
        { // Y a-t-il eu une erreur lors de la création du process fils ?
            printf("Problème durant la duplication\n");
            exit(EXIT_FAILURE);
        }
        if(i == nbForks - 1)
                printf("\n\nLes fils sont en train de tourner à l'infini via un 'while(1)' pour montrer la mémoire qu'ils occupent via la commande 'top' (cfr 'ps -aux'). Pour les arrêter, dans une autre fenêtre de terminal, entrez la commande 'kill {$PID_du_premier_fils..$PID_su_dernier_fils' !\n\n");
    }
    // Code du père
    // Cette boucle est pour éviter de créer des zombies en les tuant via un autre terminal
        for (unsigned int i = 0; i < nbForks; i++)
        {
            wait((void*)(intptr_t) forkRetNums[i]);
        }
    printf("\nLes fils sont terminés\n");
    
    printf("PID (du père, donc) = %d\n", getpid());
    printf("PPID (id du process à l'origine de la création du programme) = %d\n", getppid());

    printf("\nVu que lorsque vous tuez 1 process via un autre terminal, l'entrée  liée au process de l'affichage généré par la commande 'top' disparait, cela prouve que chaque process a bien son propre espace d'adressage.\n\n");
    
    printf ("\n\nLe programme ne se termine pas pour laisser le temps de faire un 'ps -aux' et voir quels process sont en cours d'exécution et leurs états. Pour le terminer, faites un 'kill %d' dans une autre fenêtre de terminal ou faites un CTRL + C ici\n", getpid());
    
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
 Ceci permettra d'observer que 5 lignes seront créées dans le tableau du résultat de la commande car fork() crée des process à part entière.

Pour continuer le programme, entrez 'continue' ou 'c' : c
PID du père = 98535
Ceci est le process parent et le PID est : 98535
Ceci est le process fils et le PID est : 98536
Ceci est le process parent et le PID est : 98535
Ceci est le process fils et le PID est : 98537
Ceci est le process parent et le PID est : 98535
Ceci est le process fils et le PID est : 98538
Ceci est le process parent et le PID est : 98535
Ceci est le process fils et le PID est : 98539
Ceci est le process parent et le PID est : 98535


Les fils sont en train de tourner à l'infini via un 'while(1)' pour montrer la mémoire qu'ils occupent via la commande 'top' (cfr 'ps -aux'). Pour les arrêter, dans une autre fenêtre de terminal, entrez la commande 'kill {$PID_du_premier_fils..$PID_su_dernier_fils' !

Ceci est le process fils et le PID est : 98540

Les fils sont terminés
PID (du père, donc) = 98535
PPID (id du process à l'origine de la création du programme) = 3126

Vu que lorsque vous tuez 1 process via un autre terminal, l'entrée  liée au process de l'affichage généré par la commande 'top' disparait, cela prouve que chaque process a bien son propre espace d'adressage.

Terminated

