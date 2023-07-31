// Bounded buffer with thread synchronization

#include <iostream>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

const int MAX_BUFFER_SIZE = 5;
const int NUM_PRODUCERS = 2;
const int NUM_CONSUMERS = 3;

std::queue<int> sharedQueue;
std::mutex mtx;
std::condition_variable cv;
int totalItemsProduced = 0;
int totalItemsConsumed = 0;
bool producersFinished = false;

void producerFunc(int id) {
    for (int i = 1; i <= 10; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return sharedQueue.size() < MAX_BUFFER_SIZE || producersFinished; });

        if (producersFinished) {
            break; // Exit the loop if all producers have finished producing
        }

        sharedQueue.push(i);
        ++totalItemsProduced;
        std::cout << "Producer " << id << " produced: " << i << std::endl;

        cv.notify_one();
    }
}

void consumerFunc(int id) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !sharedQueue.empty() || producersFinished; });

        if (sharedQueue.empty() && producersFinished) {
            break; // Exit the loop if the queue is empty and all producers have finished producing
        }

        if (!sharedQueue.empty()) {
            int number = sharedQueue.front();
            sharedQueue.pop();
            ++totalItemsConsumed;
            std::cout << "Consumer " << id << " consumed: " << number << std::endl;

            cv.notify_one();
        }
    }
}

int main() {
    std::vector<std::thread> producerThreads;
    std::vector<std::thread> consumerThreads;

    for (int i = 1; i <= NUM_PRODUCERS; ++i) {
        producerThreads.emplace_back(producerFunc, i);
    }

    for (int i = 1; i <= NUM_CONSUMERS; ++i) {
        consumerThreads.emplace_back(consumerFunc, i);
    }

    for (auto& t : producerThreads) {
        t.join();
    }

    {
        // Notify consumers that producers have finished
        std::unique_lock<std::mutex> lock(mtx);
        producersFinished = true;
        cv.notify_all();
    }

    for (auto& t : consumerThreads) {
        t.join();
    }

    std::cout << "Total items produced: " << totalItemsProduced << std::endl;
    std::cout << "Total items consumed: " << totalItemsConsumed << std::endl;

    return 0;
}
