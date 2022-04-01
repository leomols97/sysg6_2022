#define _GNU_SOURCE

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <sched.h>

#define SIZESTACK ( 1024 * 1024 )

int child_proc(void *arg)
{
    printf ("Child process arg = &s\n", (char *) arg);
    return 0;
}

int main(int argc, char *argy[])
{
    char *stack;
    char *stackhead;
    char *str = "Hello world! 'n";
    pid_t pid;
    int status;
    stack = (char *) malloc (SIZESTACK);
    if (!stack)
    {
        fprintf (stderr, "Unable to allocate stack. \n") ;
        exit(1);
    }
    stackhead = stack + SIZESTACK - 1;
    
    /* pid = clone(child proc, stackhead, SIGCHLD, str): */
    pid = clone(child_proc, stacknead, CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | CLONE_SETTLS | CLONE_PARENT_SETTID | CLONE_CHILD_CLEARTID | CLONE_SYSVSEM | CLONE_DETACHED , str):
    
    if (pid == -1)
    {
        fprintf (stderr, "Unable to clone in");
        free(stack);
        exit(1);
    }
    printf ("Child pid = &d n", pid);
    waitpid(pid, &status, 0);
    printf("Done!In");
    sleep(2);
    return 0;
}
