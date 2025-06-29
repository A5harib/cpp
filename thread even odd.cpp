#include<iostream>
#include<thread>
using namespace std;

void checkEvenodd(int number) {
    if (number % 2 == 0) {
        cout << "Number is even" << endl;
    } else {
        cout << "Number is odd" << endl;
    }
}

int main() {
    int num;
    cout << "Enter an integer" << endl;
    cin >> num;

    thread t(checkEvenodd, num);
    t.join();

    return 0;
}
