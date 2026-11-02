#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>

sem_t forks[5];
sem_t room;

void *philosopher(void *arg){
    int id = *(int *)arg;

    sem_wait(&room);
    printf("Philospher %d is entering the room\n",id);

    sem_wait(&forks[id]);
    printf("Philosopher %d picked the left fork\n",id);

    sem_wait(&forks[(id+1)%5]);
    printf("Philosopher %d picked the right fork\n",id);

    printf("Philosopher is eating\n");

    sleep(1);

    sem_post(&forks[(id+1)%5]);
    sem_post(&forks[id]);

    sem_post(&room);
    printf("Philosopher %d finished eating and left the room\n",id);

    return NULL;
}

int main(){
    pthread_t phils[5];
    int id[5];

    sem_init(&room,0,4);

    for(int i=0;i<5;i++){
        sem_init(&forks[i],0,1);
    }

    for(int i=0;i<5;i++){
        pthread_create(&phils[i],0,philosopher,&id[i]);
    }

    for(int i=0;i<5;i++){
        pthread_join(&phils[i],NULL);
    }
    return 0;
}
