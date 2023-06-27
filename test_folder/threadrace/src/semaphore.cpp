#include "semaphore.h"

semaphore::semaphore(unsigned int size) 
{
    this->myQueue = new std::queue<int>();
}

int semaphore::wait()
{
    int val;

    std::unique_lock<std::mutex> lk(this->myLock);
    this->myCond.wait(lk, [this] { return !myQueue->empty(); });

    val = myQueue->front();
    myQueue->pop();
    return val;
}

void semaphore::signal(int moves)
{
    std::lock_guard<std::mutex> lk(this->myLock);
    myQueue->push(moves);
    return;
}