/* Program 5: Producer-Consumer (bounded buffer)
 *  empty : counting semaphore = free slots   (starts at BUFFER_SIZE)
 *  full  : counting semaphore = filled slots (starts at 0)
 *  mutex : protects buffer, in, out
 * ORDER MATTERS: wait on empty/full FIRST, then lock the mutex.
 * Reverse order can cause deadlock (a thread sleeping while holding the mutex).
 * Compile: gcc 05_producer_consumer.c -o prod_cons -pthread
 */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE        5
#define NUM_PRODUCERS      2
#define NUM_CONSUMERS      2
#define ITEMS_PER_PRODUCER 6
#define ITEMS_PER_CONSUMER (NUM_PRODUCERS * ITEMS_PER_PRODUCER / NUM_CONSUMERS)

static int buffer[BUFFER_SIZE];
static int in = 0, out = 0;
static sem_t empty, full;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void *producer(void *arg)
{
    int id = *(int *)arg;
    for (int i = 1; i <= ITEMS_PER_PRODUCER; i++) {
        int item = id * 100 + i;
        usleep(100000 * id);                 /* producing takes time */

        sem_wait(&empty);                    /* wait for a free slot */
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
        printf("Producer %d produced %d at slot %d\n", id, item, in);
        in = (in + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);                     /* one more filled slot */
    }
    return NULL;
}

static void *consumer(void *arg)
{
    int id = *(int *)arg;
    for (int i = 0; i < ITEMS_PER_CONSUMER; i++) {
        sem_wait(&full);                     /* wait for an item */
        pthread_mutex_lock(&mutex);
        int item = buffer[out];
        printf("    Consumer %d consumed %d from slot %d\n", id, item, out);
        out = (out + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);                    /* one more free slot */

        usleep(250000);                      /* consuming takes time */
    }
    return NULL;
}

int main(void)
{
    pthread_t p[NUM_PRODUCERS], c[NUM_CONSUMERS];
    int pid[NUM_PRODUCERS], cid[NUM_CONSUMERS];

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pid[i] = i + 1;
        pthread_create(&p[i], NULL, producer, &pid[i]);
    }
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        cid[i] = i + 1;
        pthread_create(&c[i], NULL, consumer, &cid[i]);
    }
    for (int i = 0; i < NUM_PRODUCERS; i++) pthread_join(p[i], NULL);
    for (int i = 0; i < NUM_CONSUMERS; i++) pthread_join(c[i], NULL);

    printf("\nAll %d items produced and consumed.\n", NUM_PRODUCERS * ITEMS_PER_PRODUCER);
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    return 0;
}
