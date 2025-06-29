#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>

using namespace std;
using namespace chrono;

const int SIZE = 10000;
vector<int> arr(SIZE);

auto now() { return high_resolution_clock::now(); }

int totalSum = 0;
double avg = 0;
unordered_map<int, int> frequencyMap;
vector<int> mostCommon;
vector<int> missing;
mutex mtx;

void generateArray() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 20);
    for (int& x : arr) x = dis(gen);
}

void sequentialOperations() {
    auto start = now();

    int sum = 0;
    for (int x : arr) sum += x;

    double average = (double)sum / arr.size();

    unordered_map<int, int> freq;
    for (int x : arr) freq[x]++;

    int maxFreq = 0;
    vector<int> most;
    for (auto& [num, f] : freq) {
        if (f > maxFreq) {
            maxFreq = f;
            most = {num};
        } else if (f == maxFreq) {
            most.push_back(num);
        }
    }

    unordered_set<int> existing(arr.begin(), arr.end());
    vector<int> missingInRange;
    for (int i = 1; i <= 10; ++i)
        if (existing.find(i) == existing.end())
            missingInRange.push_back(i);

    auto end = now();

    cout << "Sequential Execution Time: "
         << duration_cast<milliseconds>(end - start).count()
         << " ms\n\n";
}

void computeSum(int start, int end) {
    int localSum = 0;
    for (int i = start; i < end; ++i) localSum += arr[i];
    lock_guard<mutex> lock(mtx);
    totalSum += localSum;
}

void computeFrequency(int start, int end) {
    unordered_map<int, int> localFreq;
    for (int i = start; i < end; ++i) localFreq[arr[i]]++;

    lock_guard<mutex> lock(mtx);
    for (auto& [num, f] : localFreq) frequencyMap[num] += f;
}

void computeMostCommon() {
    int maxFreq = 0;
    for (auto& [num, f] : frequencyMap)
        if (f > maxFreq) maxFreq = f;

    for (auto& [num, f] : frequencyMap)
        if (f == maxFreq)
            mostCommon.push_back(num);
}

void computeMissing() {
    unordered_set<int> existing(arr.begin(), arr.end());
    for (int i = 1; i <= 10; ++i)
        if (existing.find(i) == existing.end())
            missing.push_back(i);
}

void multithreadedExecution(int threadCount) {
    auto start = now();

    totalSum = 0;
    avg = 0;
    frequencyMap.clear();
    mostCommon.clear();
    missing.clear();

    vector<thread> threads;
    int chunk = SIZE / threadCount;

    for (int i = 0; i < threadCount; ++i) {
        int s = i * chunk;
        int e = (i == threadCount - 1) ? SIZE : s + chunk;
        threads.emplace_back(computeSum, s, e);
        threads.emplace_back(computeFrequency, s, e);
    }

    for (auto& t : threads) t.join();
    threads.clear();

    avg = (double)totalSum / SIZE;

    thread t1(computeMostCommon);
    thread t2(computeMissing);
    t1.join();
    t2.join();

    auto end = now();

    cout << "Multithreaded Execution (" << threadCount << " threads) Time: "
         << duration_cast<milliseconds>(end - start).count()
         << " ms\n\n";
}

int main() {
    generateArray();

    cout << "=== Performance Analysis ===\n\n";

    cout << "1. Sequential Version\n";
    sequentialOperations();

    cout << "2. Multithreaded with 5 threads\n";
    multithreadedExecution(5);

    cout << "3. Multithreaded with 6 threads\n";
    multithreadedExecution(6);

    return 0;
}
