#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include <stdlib.h>
#include <mutex>
#include <condition_variable>
#include <queue>

class semaphore {
    private:
        mutable std::mutex myLock;
        std::condition_variable myCond;
        std::queue<int> *myQueue;
    public:
        semaphore(unsigned int size);
        int wait();
        void signal(int moves);
};

#endif