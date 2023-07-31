// Problem 
// There are two threads one generates random number upto 10 and other thread maintains
// total sum of numbers generated so far. Synchronize both the threads
// both threads to use shared var and work in sync

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

const int gTotalNumtoGenerate = 10;
static int gRandomNumber = 0;
static int gSumOfRandomNumbers = 0;
std::mutex gMtx;
std::condition_variable gCV;

static bool Thread1Finished = false;
static bool AllNumsGenerated = false;

void ProducerFunc() {

    for(int i=0; i<gTotalNumtoGenerate; i++) {
        
        {
            std::unique_lock<std::mutex> lock(gMtx); //acquire lock
            gCV.wait(lock, [](){return !Thread1Finished;}); // wait till thread 2 has finished
            gRandomNumber = rand() % 10;
            std::cout << "Random Number generated : " << gRandomNumber << std::endl;
            Thread1Finished = true;
        }
        gCV.notify_one();
    }

    AllNumsGenerated = true;
    gCV.notify_one();

}

void ConsumerFunc() {

    while (!AllNumsGenerated) {

        {
            std::unique_lock<std::mutex> lock(gMtx);
            gCV.wait(lock, [](){return Thread1Finished;});

            gSumOfRandomNumbers += gRandomNumber;
            Thread1Finished = false;
            std::cout << " Sum of Randoms so far : " << gSumOfRandomNumbers << std::endl;
        }
        gCV.notify_one();

    }

}

int main() {
    std::thread ProducerThread(ProducerFunc);
    std::thread ConsumerThread(ConsumerFunc);

    ProducerThread.join();
    ConsumerThread.join();

    std::cout << "All threads finished " << std::endl;

    return 0;
}


