#include <iostream>
#include <thread>
using namespace std;

const int SIZE = 10;
int numbers[SIZE] = {1, 4, 5, 7, 8, 10, 13, 16, 19, 20};
int evenSum = 0;
int oddSum = 0;

void sumEven() {
    for (int i = 0; i < SIZE; i++) {
        if (numbers[i] % 2 == 0) {
            evenSum += numbers[i];
        }
    }
}

void sumOdd() {
    for (int i = 0; i < SIZE; i++) {
        if (numbers[i] % 2 != 0) {
            oddSum += numbers[i];
        }
    }
}

int main() {
    thread t1(sumEven);
    thread t2(sumOdd);

    t1.join();
    t2.join();

    cout << "Sum of even numbers: " << evenSum << endl;
    cout << "Sum of odd numbers: " << oddSum << endl;

    return 0;
}

