#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
using namespace std;

sem_t mutex;
sem_t wrt;
int readcount = 0;
int shared_data = 0;

void* reader(void* id_ptr) {
    int id = *(int*)id_ptr;

    sem_wait(&mutex);
    readcount++;
    if (readcount == 1)
        sem_wait(&wrt);
    sem_post(&mutex);

    cout << "Reader " << id << " is reading data: " << shared_data << endl;
    sleep(1);

    sem_wait(&mutex);
    readcount--;
    if (readcount == 0)
        sem_post(&wrt);
    sem_post(&mutex);

    return NULL;
}

void* writer(void* id_ptr) {
    int id = *(int*)id_ptr;

    sem_wait(&wrt);

    shared_data += 5;
    cout << "Writer " << id << " updated data to: " << shared_data << endl;
    sleep(1);

    sem_post(&wrt);

    return NULL;
}

int main() {
    pthread_t r[3], w[2];
    int r_id[] = {1, 2, 3};
    int w_id[] = {1, 2};

    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);

    pthread_create(&w[0], NULL, writer, &w_id[0]);
    pthread_create(&r[0], NULL, reader, &r_id[0]);
    pthread_create(&r[1], NULL, reader, &r_id[1]);
    pthread_create(&w[1], NULL, writer, &w_id[1]);
    pthread_create(&r[2], NULL, reader, &r_id[2]);

    for (int i = 0; i < 3; i++) pthread_join(r[i], NULL);
    for (int i = 0; i < 2; i++) pthread_join(w[i], NULL);

    return 0;
}
