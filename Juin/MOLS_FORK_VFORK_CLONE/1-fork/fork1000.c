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
int nbForks = 1;

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
 Cette fonction écrit dans un fichier une chaine reçue en paramètre

 @param fileName Le nom du fichier dans lequel écrire
 @param fileNameSize La taille du nom du fichier (en comptant l'extension) dans lequel écrire
 @param whatToWrite Ce que la fonction va écrire dans le fichier
 */
void write_into_file(char fileName[], int fileNameSize, char whatToWrite[])
{
    char nameOfFile = fileName[fileNameSize];
    FILE * forksOutFilePointer = fopen(fileName, "a");
    fputs(whatToWrite, forksOutFilePointer);
    fclose(forksOutFilePointer);
}


// Cette fonction devrait permettre de changer la valeur d'un signal d'un process
/*void signal_handler(int signal_nb)
 {
 printf("\nChange le numéro d'un signal\n");
 signal(SIGINT, SIG_DFL);
 }*/

/**
 Cette fonction a pour but d'être exécutée lorsque son nom est spécifié comme argument dans pthread_create()
 */
void *threadCreation(void *arg)
{
    printf("Fonction liée à la création de thread appelée \n");
    sleep(50); // Ceci pour permettre d'avoir le temps de prouver que le fils n'hérite pas des threads du père ni en crée de nouveaux
    return NULL;
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
    
    // Calcule la longueur du nombre 'nbForks'
    int nbForksLength = floor(log10(abs(nbForks))) + 1;
    // Calcule la taille du nom du fichier avec son extension '.txt'
    int fileNameSize = 8 + nbForksLength;
    printf("fileNameSize = %d ", fileNameSize);
    char fileName[15];
    // Construit le nom du fichier dans lequel le programme va écrire
    snprintf(fileName, sizeof(fileName), "fork%d.txt", nbForks);
    remove(fileName); // Pour avoir un fichier vide et pouvoir écrire en mode append dedans
    
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
    
    
    
    // Entiers à augmenter dans le fils pour prouver l'espace d'adressage commun
    int a = 5, b = 8;
    
    // Récupérer la valeur de retour des nbForks fork
    int forkRetNums[nbForks];
    
    printf("PID du père = %d\n", getpid());
    printf("Ces 2 variables sont créées et initialisées par le père :\n");
    printf("a = %d\n", a);
    printf("b = %d\n", b);
    
    continueProgram();
    
    printf("\nCeci est avant que le père ne crée un thread\n\n");
    
    continueProgram();
    
    pthread_t tid;
    // Crée 3 threads
    for (unsigned int i = 0; i < 3; i++)
        pthread_create(&tid, NULL, threadCreation, (void *)&tid);
    
    printf("%s", "");
    //printf("Les threads ont été créés par le père\n");
    
    //continueProgram();
    for (unsigned int i = 0; i < nbForks; i++)
    {
        write_into_file(fileName, fileNameSize, " Parent ");
        forkRetNums[i] = fork();
        
        write_into_file(fileName, fileNameSize, " Fils\n");
        if(forkRetNums[i] == 0)
        {
//            write_into_file(fileName, fileNameSize, " Fils\n");
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
    }
    // Code du père
    wait(0); // Pour éviter de faire du fils un zombie
    printf("Le fils est terminé\n");
    printf("PID (du père, donc) = %d\n", getpid());
    printf("PPID = %d\n", getppid());
    // La somme est bien de 13 et non 20 puisque la somme fut faite par le fils, mais uniquement avec ses propres variables et non celles du père
    printf("a + b = %d.\n", a + b);
    printf("Vu que a + b = 20 dans le fils et que a + b = 13 dans le père, cela prouve que l'espace d'adressage d'un process créé au moyen de fork n'est pas celui du père car il a été dupliqué par rapport à celui du père. Chaque process a donc ses propres variables,...\n");
    printf("\nDans une autre fenêtre de terminal, entrez la commande 'ps -aux' pour voir quel process est en cours et plus d'informations à leurs propos !\n\n");
    
    printf ("\n\nLe programme ne se termine pas pour laisser le temps de faire un 'ps -aux' et voir quels process sont en cours d'exécution et leurs états. Pour le terminer, faites un 'kill $PID' dans une autre fenêtre de terminal ou faites un CTRL + C\n");
    
    pthread_join(tid, NULL);
    printf("After Thread\n");
    
    while(1){} // Simplement pour faire attendre le père. Un simple 'ps -aux' montrera son état
    
    exit(0);
}
