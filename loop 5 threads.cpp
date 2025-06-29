#include <iostream>
#include <thread>
using namespace std;

void printNumber(int n) {
    cout << "Thread number: " << n << endl;
}

int main() {
    thread t[5];

    for (int i = 0; i < 5; i++) {
        t[i] = thread(printNumber, i + 1);
    }

    for (int i = 0; i < 5; i++) {
        t[i].join();
    }

    return 0;
}
