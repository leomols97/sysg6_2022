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



// Pour compiler avec les threads :
// gcc -pthread -o fork fork.c



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
void *threadCreation(void *arg)
{
    printf("Fonction liée à la création de thread appelée \n");
    sleep(50); // Ceci pour permettre d'avoir le temps de prouver que le fils n'hérite pas des threads du père ni en crée de nouveaux
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
               "L    les pages du processus sont verrouillées en mémoire;\n"
               "s    meneur de session ;\n"
               "l    possède plusieurs processus légers (« multi-thread », utilisant CLONE_THREAD comme NPTL pthreads le fait) ;\n"
               "+    dans le groupe de processus au premier plan.\n\n\n\n\n\n\n");
               
               
               
    // Entiers à augmenter dans le fils pour prouver l'espace d'adressage commun
    int a = 5, b = 8;
    // Récupérer la valeur de retour de la fonction créant le proocess fils
    int forkRetNum;
    
    printf("PID du père = %d\n", getpid());
    printf ("Ces 2 variables sont créées par le père :\n");
    printf("a = %d\n", a);
    printf("b = %d\n", b);
    
    // A tester sur les machines de l'école : il faut que la quantité de mémoire allouée au process fils soit différente de celle allouée au process père
    int dataSize = 2048;
    char dataLock[dataSize];
    if (lock_memory(dataLock, dataSize) == -1)
    	perror("Error with locking memory\n");
    else
    	printf ("\nDe la mémoire a été réservée en RAM par le père\n");

    	
    /*signal(SIGINT, signal_handler);
    sleep(30);
    for(int i=1; ; i++)
    {
   	printf("%d, Le programme est dans la fonction main\n", i);
    	sleep(1);
    }*/
    
    continueProgram();
    
    printf("\nCeci est avant que le père ne crée un thread\n\n");
    continueProgram();
    
    int i;
    pthread_t tid;
    // Let us create three threads
    for (i = 0; i < 3; i++)
        pthread_create(&tid, NULL, threadCreation, (void *)&tid);
 
    printf("");
    //printf("Le thread a été créé par le père\n");
    
    //continueProgram();
    forkRetNum = fork();
        
    if(forkRetNum == 0) { // La création du fils s'est-elle correctement produite ?
        printf("Le processus fils vient d'être créé. La suite est affichée par le fils.\n");
        // a = 10 but only the one of the chile. not the one of the parent
        a = a + 5;
        printf("Maintenant, a = %d et ce, uniquement dans l'espace d'adressage du fils\n", a);

        // b = 10 but only the one of the chile. not the one of the parent
        b = b + 2;
        printf("Maintenant, b = %d et ce, uniquement dans l'espace d'adressage du fils\n", b);

	//lock_memory();

        printf("PID (du fils, donc) = %d\n", getpid());
        printf("PID du père = %d\n", getppid());
        printf("a + b = %d.\n", a + b);
        printf("\nDans une autre fenêtre de terminal, entrez la commande 'ps' pour voir quel process est en cours et plus d'informations à leurs propos !\n\n");
        printf("Sous la section 'Status', vous pouvez voir que le statut du père est 'SLl+'. Le 'L' signifie que de la mémoire est verrouillée en RAM par le process !\n\n");
        printf("RSS signifie Resident Set Size et montre la quantité de RAM utilisée au moment de la sortie de la commande. "
        	"Il convient également de noter qu'il affiche toute la pile de mémoire physiquement allouée.\n\n");
        printf("VSZ est l'abréviation de Virtual Memory Size. C'est la quantité totale de mémoire à laquelle un processus peut hypothétiquement accéder. "
                "Il tient compte de la taille du binaire lui-même, de toutes les bibliothèques liées et de toutes les allocations de pile ou de tas.\n");
        printf("\n\nDans une autre fenêtre de terminal, entrez la commande 'cat /proc/$PID/status' pour voir les informations du process !\n");
        
        continueProgram();
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
    printf ("Le fils est terminé\n");
    printf("PID = %d\n", getpid());
    printf("PPID = %d\n", getppid());
    // La somme est bien de 13 et non plus ni moins puisque la somme fut faite par le fils, mais uniquement avec ses propres variables et non celles du père
    printf("a + b = %d.\n", a + b);
    printf("Vu que a + b = 20 dans le fils et que a + b = 13 dans le père, cela prouve que l'espace d'adressage d'un process créé au moyen de fork n'est pas celui du père car il a été dupliqué par rapport à celui du père. Chaque process a donc ses propres variables,...\n");
    printf("Let's do a ps to see which process is currenlty running !");
    
    if (unlock_memory(dataLock, dataSize) == -1)
    	perror("Error with locking memory\n");
    else
    	printf ("Memory unlocked in RAM\n");
    	
    printf ("\n\nLe programme ne se termine pas pour laisser le temps de faire un ps et voir quels process sont en cours d'exécution. Pour le terminer, faites un 'kill $PID' dans une autre fenêtre de terminal ou faites un CTRL + C\n");
        
    pthread_join(tid, NULL);
    printf("After Thread\n");
  
    while(1){} // Simplement pour faire attendre le père que l'on fasse un ps pour pouvoir voir son état
  
    exit(0);
}
