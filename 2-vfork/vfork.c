#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t L1;

f1() /* This is executed just before the process forks. */
{
    mutex_lock(L1); |
    mutex_lock(...); | -- ordered in lock order
    mutex_lock(Ln); |
} V

f2() /* This is executed in the child after the process forks. */
{
    mutex_unlock(L1);
    mutex_unlock(...);
    mutex_unlock(Ln);
}

f3() /* This is executed in the parent after the process forks. */
{
    mutex_unlock(L1);
    mutex_unlock(...);
    mutex_unlock(Ln);
}

int main()
{
    printf ("Before Forking\n");
    pid_t vfork();
    printf ("After Forking\n");

    mutex_lock(L1);
    mutex_lock(L2);
    fork1(...);
    mutex_unlock(L1);
    mutex_unlock(L2);
}
