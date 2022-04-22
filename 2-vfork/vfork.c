#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <mm_malloc.h>
#include <spawn.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {

    int a = 5, b = 8;
    int vforkRetNum;

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
    
    
    vforkRetNum = vfork();
    if(vforkRetNum == 0) {
        printf("I'm the child !\n");
        // a = 10
        a = a + 5;
        // b = 10
        b = b + 2;
        
        printf("PID = %d\n", getpid());
        printf("PPID = %d\n", getppid());
        //printf("Value of vfork is %d.\n", vforkRetNum); // line a
        printf("Sum is %d.\n", a + b); // line b
        printf("Let's do a ps to see which process is currenlty running !\n");
        printf("Let's send kill -9 %d to try to kill the parent before the child exits !\n\n", getppid());
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
        sleep(10);
        exit(0);
    }
    // Parent code
    wait(0);
    printf("PID = %d\n", getpid());
    printf("PPID = %d\n", getppid());
    //printf("Value of vfork is %d.\n", vforkRetNum); // line a
    printf("Sum is %d.\n", a + b); // line b
    printf("Let's do a ps to see which process is currenlty running !\n");
    sleep(10);
    exit(0);

}
