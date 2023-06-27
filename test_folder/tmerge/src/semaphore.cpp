#include "semaphore.h"

semaphore::semaphore(unsigned int size)
{
    std::lock_guard<std::mutex> lk(this->myLock);
    this->counter = size;
}

void semaphore::signal()
{
    std::lock_guard<std::mutex> lk(this->myLock);
    counter = counter + 2;
    myCond.notify_all();
}

void semaphore::wait()
{
    std::unique_lock<std::mutex> lk(this->myLock);
    this->myCond.wait(lk, [this]{ return counter > 0; });
    counter = counter - 2;
    lk.unlock();
    return;
}