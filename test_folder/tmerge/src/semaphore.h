#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include <stdlib.h>
#include <mutex>
#include <condition_variable>

class semaphore {
    private:
        unsigned int counter;
        mutable std::mutex myLock;
        std::condition_variable myCond;
    
    public:
        semaphore(unsigned int size);
        void signal();
        void wait();
};

#endif