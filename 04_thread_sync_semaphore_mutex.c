4
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t lock;
sem_t sem;

void *work(void *arg) {
    int id = *(int *)arg;

    sem_wait(&sem);
    pthread_mutex_lock(&lock);

    printf("Thread %d is working\n", id);

    pthread_mutex_unlock(&lock);
    sem_post(&sem);

    return NULL;
}

int main() {
    pthread_t t[3];
    int id[3];

    pthread_mutex_init(&lock, NULL);
    sem_init(&sem, 0, 2);

    for (int i = 0; i < 3; i++) {
        id[i] = i + 1;
        pthread_create(&t[i], NULL, work, &id[i]);
    }

    for (int i = 0; i < 3; i++)
        pthread_join(t[i], NULL);

    pthread_mutex_destroy(&lock);
    sem_destroy(&sem);

    return 0;
}
