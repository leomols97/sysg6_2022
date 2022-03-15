#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <mm_malloc.h>
#include <spawn.h>

void closeall (int p1 [], int p2[]){
        close (p1 [0]);
        close (p1 [1]);
        close (p2 [0]);
        close (p2 [1]);
    }

int main()
{
    int a = 10;
    //int * adress = malloc(sizeof(int));
    //*adress = 10;
    printf ("Before Forking\n");
    printf ("%d\n", a);
    //printf ("%d\n", adress);
    if (fork() == 0)
    {
        a = 20;
        //exit(0);
    }
    //printf ("%d", getpid());
    printf ("After Forking\n");
    printf ("%d\n", a);
    //printf ("%d\n", adress);
}
