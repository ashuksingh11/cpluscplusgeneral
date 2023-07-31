//Thread race can occur when different threads try to run and access 
// shared resource at run time
//This exmaple shows change numOfthreads to simulate in your own environment

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

static int gSharedVar = 0;
std::mutex gMtx;

//this is shared func between all threads that increment a shared var
void IncrementSharedCounter() {
    // gaurd using mutex
    // comment this line to see effects of no synchronization
    // lock gaurd is RAII based sync primitive no need to specifically lock and unlock mutex
    std::lock_guard<std::mutex> lock(gMtx);
    gSharedVar = gSharedVar + 1;
}

int main() {
    std::vector<std::thread> Threads;
    const int NumOfThreads = 2000;

    for(int i=0; i<NumOfThreads; i++) {
        Threads.push_back(std::thread(IncrementSharedCounter));
    }

    for(int i=0; i<NumOfThreads; i++) {
        Threads[i].join();
    }

    std::cout << "Shared Counter val ; " << gSharedVar << std::endl;
}