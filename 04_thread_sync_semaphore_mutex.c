/* Program 4: Thread synchronization
 *  - COUNTING SEMAPHORE : only MAX_SLOTS threads may be inside the "resource
 *                         section" at the same time (limits concurrency).
 *  - MUTEX              : protects shared variables (mutual exclusion).
 * Compile: gcc 04_thread_sync_semaphore_mutex.c -o thread_sync -pthread
 */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 8
#define MAX_SLOTS   3
#define INCREMENTS  10000

static sem_t slots;                                   /* counting semaphore */
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static int  active = 0, max_active = 0;               /* shared */
static long shared_counter = 0;                       /* shared */

static void *worker(void *arg)
{
    int id = *(int *)arg;

    sem_wait(&slots);                                 /* take one slot (blocks if 0) */

    pthread_mutex_lock(&lock);
    active++;
    if (active > max_active) max_active = active;
    printf("Thread %d ENTERED  (threads inside = %d)\n", id, active);
    pthread_mutex_unlock(&lock);

    usleep(300000);                                   /* pretend to use the resource */

    for (int i = 0; i < INCREMENTS; i++) {            /* critical section */
        pthread_mutex_lock(&lock);
        shared_counter++;
        pthread_mutex_unlock(&lock);
    }

    pthread_mutex_lock(&lock);
    active--;
    printf("Thread %d LEFT     (threads inside = %d)\n", id, active);
    pthread_mutex_unlock(&lock);

    sem_post(&slots);                                 /* give the slot back */
    return NULL;
}

int main(void)
{
    pthread_t t[NUM_THREADS];
    int ids[NUM_THREADS];

    sem_init(&slots, 0, MAX_SLOTS);                   /* initial value = 3 */

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i + 1;
        pthread_create(&t[i], NULL, worker, &ids[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(t[i], NULL);

    printf("\nMax threads inside at one time : %d (limit %d)\n", max_active, MAX_SLOTS);
    printf("Shared counter                 : %ld (expected %d)\n",
           shared_counter, NUM_THREADS * INCREMENTS);

    sem_destroy(&slots);
    pthread_mutex_destroy(&lock);
    return 0;
}
