#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
using namespace std;

const int SIZE = 5;
int buffer[SIZE];
int in = 0, out = 0;

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

void* producer(void*) {
    int item = 1;
    do {
        cout << "Producing item: " << item << endl;

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        cout << "Added to buffer at index " << in << endl;
        in = (in + 1) % SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);

        item++;
        sleep(1);
    } while (true);
    return NULL;
}

void* consumer(void*) {
    int item;
    do {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        item = buffer[out];
        cout << "Removed from buffer at index " << out << endl;
        out = (out + 1) % SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        cout << "Consumed item: " << item << endl;
        sleep(1);
    } while (true);
    return NULL;
}

int main() {
    pthread_t prod, cons;

    sem_init(&empty, 0, SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}
