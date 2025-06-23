#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
using namespace std;

const int N = 5;
sem_t chopstick[N];

void* philosopher(void* arg) {
    int i = *(int*)arg;

    do {
        sem_wait(&chopstick[i]);
        sem_wait(&chopstick[(i + 1) % N]);

        cout << "Philosopher " << i << " is eating\n";
        sleep(1);

        sem_post(&chopstick[i]);
        sem_post(&chopstick[(i + 1) % N]);

        cout << "Philosopher " << i << " is thinking\n";
        sleep(1);

    } while (true);

    return NULL;
}

int main() {
    pthread_t phil[N];
    int id[N];

    for (int i = 0; i < N; i++) {
        sem_init(&chopstick[i], 0, 1);
        id[i] = i;
    }

    for (int i = 0; i < N; i++)
        pthread_create(&phil[i], NULL, philosopher, &id[i]);

    for (int i = 0; i < N; i++)
        pthread_join(phil[i], NULL);

    return 0;
}
