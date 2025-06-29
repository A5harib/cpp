#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

void doWork(int threadNum, int seconds) {
    cout << "Thread " << threadNum << " started working..." << endl;
    this_thread::sleep_for(chrono::seconds(seconds));
    cout << "Thread " << threadNum << " finished after " << seconds << " seconds." << endl;
}

int main() {
    thread t1(doWork, 1, 2);
    thread t2(doWork, 2, 3);
    thread t3(doWork, 3, 1);

    t1.join();
    t2.join();
    t3.join();

    cout << "All threads completed. Main thread exiting." << endl;
    return 0;
}
