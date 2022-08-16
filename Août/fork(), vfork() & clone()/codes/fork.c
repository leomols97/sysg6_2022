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



// Pour compiler avec les threads :
// gcc -pthread -o fork fork.c


// Cette fonction permet à l'utilisateur de choisir à quel moment reprendre
// l'exécution du programme pour lui laisser le temps de faire les manipulations qu'il désire
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

// Cette fonction permet de réserver de la mémoire en RAM
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

// Cette fonction permet de libérer la mémoire réservée au process en question
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

// Cette fonction devrait permettre de changer la valeur d'un signal d'un process
/*void signal_handler(int signal_nb)
{
	printf("\nChange le numéro d'un signal\n");
	signal(SIGINT, SIG_DFL);
}*/

// Cette fonction a pour but d'être exécutée
// lorsque son nom est spécifié comme argument dans pthread_create()
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
    printf ("Ces 2 variables sont créées et initialisées par le père :\n");
    printf("a = %d\n", a);
    printf("b = %d\n", b);
    
    int dataSize = 2048;
    char dataLock[dataSize];
    if (lock_memory(dataLock, dataSize) == -1)
    	perror("Error with locking memory\n");
    else
    	printf ("\nDe la mémoire a été réservée en RAM par le père\n");
    
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
    forkRetNum = fork();
        
    if(forkRetNum == 0)
    { // La création du fils s'est-elle correctement produite ?
        printf("Le processus fils vient d'être créé. La suite est affichée par le fils.\n");
        // a = 10 mais seulement la variable 'a' du fils et non celle du père
        a = a + 5;
        printf("Maintenant, a = %d et ce, uniquement dans l'espace d'adressage du fils\n", a);

        // b = 10 mais seulement la variable 'b' du fils et non celle du père
        b = b + 2;
        printf("Maintenant, b = %d et ce, uniquement dans l'espace d'adressage du fils\n", b);

	//lock_memory();

        printf("PID (du fils, donc) = %d\n", getpid());
        printf("PID du père = %d\n", getppid());
        printf("a + b = %d.\n", a + b);
        printf("\nDans une autre fenêtre de terminal, entrez la commande 'ps -aux' pour voir quel process est en cours et plus d'informations à leurs propos !\n\n");
        printf("Sous la section 'Status', vous pouvez voir que le statut du père est 'SLl+'.\nLe 'L' signifie que de la mémoire est verrouillée en RAM par le process !\nLe 'l' signifie que le proccess possède plusieurs processus légers : les threads qu'il a créés\n\n");
        printf("RSS signifie Resident Set Size et montre la quantité de RAM utilisée au moment de la sortie de la commande. "
        	"Il convient également de noter qu'il affiche toute la pile de mémoire physiquement allouée.\n\n");
        printf("VSZ est l'abréviation de Virtual Memory Size. C'est la quantité totale de mémoire à laquelle un processus peut hypothétiquement accéder. "
                "Il tient compte de la taille du binaire lui-même, de toutes les bibliothèques liées et de toutes les allocations de pile ou de tas.\n");
        printf("\n\nDans une autre fenêtre de terminal, entrez la commande 'cat /proc/$PID/status' pour voir les informations du process !\n");
        
        printf("\n\nLe fils est en train de tourner à l'infini via un 'while(1)' pour prouver qu'il n'est pas en sommeil (cfr 'ps -aux'). Pour l'arrêter, dans une autre fenêtre de terminal, entrez la commande 'kill $PID' !\n");
        printf("\n\nCe signal indique qu'un processus fils s'est arrêté ou a fini son exécution. Par défaut ce signal est ignoré. SIGHUP : n°1");
        printf("\n\nUn processus qui effectue une division par zéro reçoit un signal SIGFPE : n°8");
        
        //continueProgram();
        while(1){} // Faire en sorte que le fils attende, mais en étant en état d'exécution. Un simple 'ps -aux' le montrera
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
    // Code du père
    wait(0); // Pour éviter de faire du fils un zombie
    printf ("Le fils est terminé\n");
    printf("PID (du père, donc) = %d\n", getpid());
    printf("PPID = %d\n", getppid());
    // La somme est bien de 13 et non 20 puisque la somme fut faite par le fils, mais uniquement avec ses propres variables et non celles du père
    printf("a + b = %d.\n", a + b);
    printf("Vu que a + b = 20 dans le fils et que a + b = 13 dans le père, cela prouve que l'espace d'adressage d'un process créé au moyen de fork n'est pas celui du père car il a été dupliqué par rapport à celui du père. Chaque process a donc ses propres variables,...\n");
    printf("\nDans une autre fenêtre de terminal, entrez la commande 'ps -aux' pour voir quel process est en cours et plus d'informations à leurs propos !\n\n");
    
    if (unlock_memory(dataLock, dataSize) == -1)
    	perror("Error with locking memory\n");
    else
    	printf ("Memory unlocked in RAM\n");
    	
    printf ("\n\nLe programme ne se termine pas pour laisser le temps de faire un 'ps -aux' et voir quels process sont en cours d'exécution et leurs états. Pour le terminer, faites un 'kill $PID' dans une autre fenêtre de terminal ou faites un CTRL + C\n");

    pthread_join(tid, NULL);
    printf("After Thread\n");
  
    while(1){} // Simplement pour faire attendre le père. Un simple 'ps -aux' montrera son état
  
    exit(0);
}
