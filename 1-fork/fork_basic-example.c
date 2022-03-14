#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <mm_malloc.h>

int main()
{
    printf ("Before Forking\n");
    vfork();
    printf ("%d", getpid());
    printf ("After Forking\n");
}
