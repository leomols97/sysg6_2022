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



// Pour compiler avec les threads :
// gcc -pthread -o fork fork.c



void continueProgram()
{
	printf("\nPour continuer le programme, entrez 'c'\n");
	int c, numberCounter = 0, letterCounter = 0;
while ((c = getchar()) != 'c')
{
    if (isalpha(c))
    {
        letterCounter++;
    }
    else if (isdigit(c))
    {
        numberCounter++;
    }
}

}

int lock_memory(char * address, size_t size)
{
	//https://linuxhint.com/mlock-2-c-function/
	unsigned long page_offset, page_size;
	page_size = sysconf(_SC_PAGE_SIZE);
	page_offset = (unsigned long) address % page_size;
	address -= page_offset; // adjust address to pageboundary
	size += page_offset; // adjust size with page_offset
	return (mlock(address, size));
}

int unlock_memory(char * address, size_t size)
{
	//https://linuxhint.com/mlock-2-c-function/
	unsigned long page_offset, page_size;
	page_size = sysconf(_SC_PAGE_SIZE);
	page_offset = (unsigned long) address % page_size;
	address -= page_offset; // adjust address to pageboundary
	size += page_offset; // adjust size with page_offset
	return (munlock(address, size));
}

void signal_handler(int signal_nb)
{
	printf("\nChange le numéro d'un signal\n");
	signal(SIGINT, SIG_DFL);
}

// A normal C function that is executed as a thread
// when its name is specified in pthread_create()
void *threadCreation(void *vargp)
{
    sleep(30);
    printf("Printing GeeksQuiz from Thread \n");
    return NULL;
}

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
               "L    avec ses pages verrouillées en mémoire (pour temps réel et entrées et sorties personnalisées) ;\n"
               "s    meneur de session ;\n"
               "l    possède plusieurs processus légers (« multi-thread », utilisant CLONE_THREAD comme NPTL pthreads le fait) ;\n"
               "+    dans le groupe de processus au premier plan.\n\n\n\n\n\n\n");
               
               
               
    // Entiers à augmenter dans le fils pour prouver l'espace d'adressage commun
    int a = 5, b = 8;
    // Récupérer la valeur de retour de la fonction créant le proocess fils
    int forkRetNum;
    
    printf("a = %d\n", a);
    printf("b = %d\n", b);
    
    // A tester sur les machines de l'école : il faut que la quantité de mémoire allouée au process fils soit différente de celle allouée au process père
    int dataSize = 2048;
    char dataLock[dataSize];
    if (lock_memory(dataLock, dataSize) == -1)
    	perror("Error with locking memory\n");
    else
    	printf ("Memory locked in RAM\n");

    	
    /*signal(SIGINT, signal_handler);
    sleep(30);
    for(int i=1; ; i++)
    {
   	printf("%d, Le programme est dans la fonction main\n", i);
    	sleep(1);
    }*/
    
    continueProgram();
    
    pthread_t thread_id;
    printf("Before creating a thread with the parent\n");
    pthread_create(&thread_id, NULL, threadCreation, NULL);
    printf("Thread created with the parent\n");
    
    forkRetNum = fork();
        
    if(forkRetNum == 0) { // La création du fils s'est-elle correctement produite ?
        printf("I'm the child !\n");
        // a = 10 but only the one of the chile. not the one of the parent
        a = a + 5;
        printf("Now, a = %d only in the child\n", a);

        // b = 10 but only the one of the chile. not the one of the parent
        b = b + 2;
        printf("Now, b = %d only in the child\n", b);

	//lock_memory();

        printf("PID = %d\n", getpid());
        printf("PPID = %d\n", getppid());
        printf("Sum a + b is %d.\n", a + b); // line b
        printf("Dans une autre fenêtre de terminal, entrez la commande 'ps' pour voir quel process est en cours et plus d'informations à leurs propos !\n\n");
        printf("Dans une autre fenêtre de terminal, entrez la commande 'cat /proc/$PID/status | grep VmLck' pour voir quel memory lock est alloué au process !\n\n");
        printf("Dans une autre fenêtre de terminal, entrez la commande 'cat /proc/$PID/status' pour voir les informations du process !\n");
        printf("VSZ est l'abréviation de Virtual Memory Size. C'est la quantité totale de mémoire à laquelle un processus peut hypothétiquement accéder."
               + "Il tient compte de la taille du binaire lui-même, de toutes les bibliothèques liées et de toutes les allocations de pile ou de tas.\n");
        printf("RSS signifie Resident Set Size et montre la quantité de RAM utilisée au moment de la sortie de la commande.
               + "Il convient également de noter qu'il affiche toute la pile de mémoire physiquement allouée.\n\n");
        
        sleep(30);
        //while(1){} // Faire en sorte que le fils attende, mais en étant en état d'exécution. Un simple ps le montrera
        exit(0);
    }
    else if (forkRetNum > 0)
    {  // Est-ce le process parent ?
        printf("Ceci est le process parent et le PID est : %d\n", getpid());
    }
    else
    { // Y a-t-il eu une erreur lors de la création du process fils ?
        printf("Problème durant la duplication\n");
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
    
    if (unlock_memory(dataLock, dataSize) == -1)
    	perror("Error with locking memory\n");
    else
    	printf ("Memory unlocked in RAM\n");
    	
    printf ("\n\nLe programme ne se termine pas pour laisser le temps de faire un ps et voir quels process sont en cours d'exécution. Pour le terminer, faites un 'kill $PID' dans une autre fenêtre de terminal ou faites un CTRL + C\n");
        
    pthread_join(thread_id, NULL);
    printf("After Thread\n");
  
    while(1){} // Simplement pour faire attendre le père que l'on fasse un ps pour pouvoir voir son état
  
    exit(0);
}
