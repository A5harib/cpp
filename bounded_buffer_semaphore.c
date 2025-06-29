#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
int itemID = 0;

sem_t emptySlots;
sem_t fullSlots;
pthread_mutex_t mtx;

void* producer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        int item;

        item = ++itemID;

        sem_wait(&emptySlots);
        pthread_mutex_lock(&mtx);

        buffer[in] = item;
        printf("Producer %d produced item %d\n", id, item);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mtx);
        sem_post(&fullSlots);

        sleep(1);
    }
    return NULL;
}

void* consumer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        int item;

        sem_wait(&fullSlots);
        pthread_mutex_lock(&mtx);

        item = buffer[out];
        printf("Consumer %d consumed item %d\n", id, item);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mtx);
        sem_post(&emptySlots);

        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t producers[2], consumers[2];
    int ids[2] = {1, 2};

    sem_init(&emptySlots, 0, BUFFER_SIZE);
    sem_init(&fullSlots, 0, 0);
    pthread_mutex_init(&mtx, NULL);

    for (int i = 0; i < 2; i++) {
        pthread_create(&producers[i], NULL, producer, &ids[i]);
        pthread_create(&consumers[i], NULL, consumer, &ids[i]);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }

    sem_destroy(&emptySlots);
    sem_destroy(&fullSlots);
    pthread_mutex_destroy(&mtx);

    return 0;
}
