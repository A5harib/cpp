#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <semaphore.h>
#include <mutex>
#include <unistd.h>

using namespace std;

const int BUFFER_SIZE = 5;
queue<int> buffer;

sem_t emptySlots;
sem_t fullSlots;
mutex mtx;

int itemID = 0;

void producer(int id) {
    while (true) {
        int item = ++itemID;
        sem_wait(&emptySlots);
        {
            lock_guard<mutex> lock(mtx);
            buffer.push(item);
            cout << "Producer " << id << " produced item " << item << endl;
        }
        sem_post(&fullSlots);
        sleep(1);
    }
}

void consumer(int id) {
    while (true) {
        sem_wait(&fullSlots);
        int item;
        {
            lock_guard<mutex> lock(mtx);
            item = buffer.front();
            buffer.pop();
            cout << "Consumer " << id << " consumed item " << item << endl;
        }
        sem_post(&emptySlots);
        sleep(2);
    }
}

int main() {
    sem_init(&emptySlots, 0, BUFFER_SIZE);
    sem_init(&fullSlots, 0, 0);

    vector<thread> producers, consumers;

    for (int i = 1; i <= 2; i++)
        producers.emplace_back(producer, i);

    for (int i = 1; i <= 2; i++)
        consumers.emplace_back(consumer, i);

    for (auto& p : producers) p.join();
    for (auto& c : consumers) c.join();

    sem_destroy(&emptySlots);
    sem_destroy(&fullSlots);

    return 0;
}

