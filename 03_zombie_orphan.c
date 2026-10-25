/* Program 3: Zombie and Orphan processes
 * Zombie : child has exited, but parent has not yet called wait().
 * Orphan : parent exits while child is still running; init/systemd adopts it.
 * Compile: gcc 03_zombie_orphan.c -o zombie_orphan
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static void zombie_demo(void)
{
    pid_t pid = fork();

    if (pid < 0) { perror("fork"); return; }

    if (pid == 0) {
        printf("[Child  %d] I am exiting immediately.\n", getpid());
        exit(0);
    }

    char cmd[128];
    printf("[Parent %d] Not calling wait() yet. Sleeping 3 seconds...\n", getpid());
    sleep(3);
    printf("[Parent] Child %d state below. 'Z' (defunct) means ZOMBIE:\n", pid);
    snprintf(cmd, sizeof cmd, "ps -o pid,ppid,stat,comm -p %d", pid);
    fflush(stdout);
    system(cmd);

    wait(NULL);                     /* reaps the zombie */
    printf("[Parent] wait() called. Zombie removed. Checking again:\n");
    fflush(stdout);
    system(cmd);                    /* process no longer listed */
}

static void orphan_demo(void)
{
    pid_t pid = fork();

    if (pid < 0) { perror("fork"); return; }

    if (pid == 0) {
        printf("[Child  %d] My parent is %d\n", getpid(), getppid());
        fflush(stdout);
        sleep(4);                   /* parent dies during this time */
        printf("[Child  %d] I am an ORPHAN now. My new parent is %d (init/systemd)\n",
               getpid(), getppid());
        exit(0);
    }
    printf("[Parent %d] Exiting immediately, child %d keeps running.\n", getpid(), pid);
    exit(0);
    /* Note: your shell prompt will return before the child prints its last line. */
}

int main(void)
{
    int choice;

    printf("1. Zombie process demo\n2. Orphan process demo\nChoice: ");
    if (scanf("%d", &choice) != 1)
        return 1;
    fflush(stdout);

    if (choice == 1)      zombie_demo();
    else if (choice == 2) orphan_demo();
    else                  printf("Invalid choice.\n");
    return 0;
}
