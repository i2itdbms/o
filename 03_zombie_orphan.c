#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;

    // 3A: Zombie Process
    pid = fork();

    if(pid == 0)
    {
        printf("Zombie: Child terminated\n");
    }
    else
    {
        printf("Zombie: Parent PID = %d\n", getpid());
        printf("Child PID = %d\n", pid);

        sleep(5);
        wait(NULL);

        printf("Child reaped\n");
    }

    // 3B: Orphan Process
    pid = fork();

    if(pid == 0)
    {
        sleep(5);
        printf("\nOrphan: Child PID = %d\n", getpid());
        printf("New Parent PID = %d\n", getppid());
    }
    else
    {
        printf("\nOrphan: Parent PID = %d\n", getpid());
        printf("Parent terminating\n");
    }

    return 0;
}
