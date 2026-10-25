/* Program 6: Dining Philosophers -- deadlock-free solution using semaphores
 *
 * NAIVE version  : each philosopher picks left chopstick, then right.
 *                  If all 5 pick their left at the same time -> every one waits
 *                  for the right chopstick forever = DEADLOCK (circular wait).
 * SAFE version   : a counting semaphore "room" (value N-1) lets at most 4
 *                  philosophers try to eat together. With 5 chopsticks and
 *                  only 4 competitors, at least one can always get both
 *                  chopsticks, so circular wait is impossible.
 * STARVATION note: this removes deadlock, but sem_wait() on Linux is not
 *                  guaranteed FIFO, so in theory a philosopher could keep
 *                  losing. A fair queue (or ordered fork picking) is needed
 *                  for a strict guarantee.
 *
 * Compile: gcc 06_dining_philosophers.c -o philosophers -pthread
 * Run    : ./philosophers          (safe)
 *          ./philosophers naive    (shows deadlock, auto-exits after 5 sec)
 */
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>

#define N      5
#define ROUNDS 3

static sem_t chopstick[N];
static sem_t room;
static int naive = 0;

static void on_alarm(int sig)
{
    (void)sig;
    const char msg[] = "\n*** DEADLOCK DETECTED: no philosopher made progress for 5 seconds ***\n";
    write(STDOUT_FILENO, msg, sizeof msg - 1);
    _exit(1);
}

static void *philosopher(void *arg)
{
    int id = *(int *)arg;
    int left = id, right = (id + 1) % N;

    for (int r = 1; r <= ROUNDS; r++) {
        printf("Philosopher %d is THINKING (round %d)\n", id, r);
        if (!naive) usleep(100000 * (id + 1));

        if (!naive) sem_wait(&room);               /* at most N-1 enter */

        sem_wait(&chopstick[left]);
        printf("Philosopher %d picked LEFT  chopstick %d\n", id, left);
        usleep(100000);                            /* widens the deadlock window */
        sem_wait(&chopstick[right]);
        printf("Philosopher %d picked RIGHT chopstick %d\n", id, right);

        printf("Philosopher %d is EATING\n", id);
        usleep(200000);

        sem_post(&chopstick[right]);
        sem_post(&chopstick[left]);
        printf("Philosopher %d put down both chopsticks\n", id);

        if (!naive) sem_post(&room);
    }
    printf("Philosopher %d is DONE\n", id);
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t t[N];
    int ids[N];

    if (argc > 1 && strcmp(argv[1], "naive") == 0) {
        naive = 1;
        signal(SIGALRM, on_alarm);
        alarm(5);
        printf("Running NAIVE version (deadlock expected)\n\n");
    } else {
        printf("Running DEADLOCK-FREE version\n\n");
    }

    for (int i = 0; i < N; i++) sem_init(&chopstick[i], 0, 1);
    sem_init(&room, 0, N - 1);

    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&t[i], NULL, philosopher, &ids[i]);
    }
    for (int i = 0; i < N; i++) pthread_join(t[i], NULL);

    printf("\nAll philosophers finished without deadlock.\n");
    return 0;
}
