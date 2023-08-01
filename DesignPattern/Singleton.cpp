#include <memory>
#include <iostream>
#include <mutex>

class Singleton {
    static std::weak_ptr<Singleton> mInstance;
    static std::mutex mMutex;
    int mcounter;

    Singleton(){mcounter++;};
    Singleton(const Singleton& ) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(const Singleton&&) = delete;
    Singleton& operator=(const Singleton&&) = delete;

    public:
    ~Singleton() {std::cout << mcounter << " singleton destroyed" << std::endl;}

    static std::shared_ptr<Singleton> GetInstance() {
        std::shared_ptr<Singleton> shared = mInstance.lock();

        if(shared == nullptr) {
            std::lock_guard<std::mutex> lock(mMutex);
            if(shared == nullptr) {
                shared = std::shared_ptr<Singleton>(new Singleton());
            }
        }

        return shared;
    }

};

std::weak_ptr<Singleton> Singleton::mInstance;
std::mutex Singleton::mMutex;

int main() {

    {
        std::shared_ptr<Singleton> ptr1 = Singleton::GetInstance();
        std::shared_ptr<Singleton> ptr2 = ptr1;
        std::shared_ptr<Singleton> ptr3 = Singleton::GetInstance();

        std::cout << " ptr1 use_count : " << ptr1.use_count() << " ptr2 use_count : " << ptr2.use_count() 
        << " ptr3 use_count : " << ptr3.use_count()<< std::endl;
    }
}