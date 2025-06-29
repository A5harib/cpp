#include<iostream>
#include<thread>
using namespace std;

void messagePrint1() {
    cout << "Hello i am thread 1" << endl;
}

void messagePrint2() {
    cout << "Hello i am thread 2" << endl;
}

int main() {
    thread t1(messagePrint1);
    thread t2(messagePrint2);

    t1.join();
    t2.join();

    cout << "Both threads printed their messages successfully" << endl;
    return 0;
}
