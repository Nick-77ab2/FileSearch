#include "race.h"
#include "semaphore.h"

// global vars
int num_racers;
semaphore *sem;
std::queue<int> *dice_queue;
std::queue<std::thread::id> *win_queue;

// Lock for io output
std::mutex io_lock;
// Lock for counting remaining racers
std::mutex count_lock;

// Thread safe printing
// print 1 line (with eol) to stdout
void print_stuff(const char *stuff);

void game_master()
{
    int i, k;
    srand(time(NULL));
    
    // if there's still racer, keep adding moves to the queue
    while (1) {
        {
            std::lock_guard lk(count_lock);
            if (num_racers < 1)
                return;
        }

        k = rand() % 6;
        for (i = 0; i < k; i++) {
            sem->signal(rand() % 6 + 1);
        }
    }

    return;
}

void racer()
{
    int moves, total_moves;
    char print_buf[256];
    srand(time(NULL));

    // Left the gate
    {
        std::lock_guard<std::mutex> lk(io_lock);
        std::cout << "Thread " << std::this_thread::get_id() << " has left the gate." << std::endl;
    }

    // Racing
    while (total_moves < 20) {
        moves = sem->wait();
        total_moves += moves;
        
        {
            std::lock_guard<std::mutex> lk(io_lock);
            std::cout << "Thread " << std::this_thread::get_id() << " moved forwards " << moves << " spaces." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(rand() % 3));
    }

    {
        std::lock_guard<std::mutex> lk(count_lock);
        num_racers--;
        win_queue->push(std::this_thread::get_id());
    }

    {
        std::lock_guard<std::mutex> lk(io_lock);
        std::cout << "Thread " << std::this_thread::get_id() << " has crossed the finish line." << std::endl;
    }
    return;
}

template<typename FirstArg, typename ... Args>
void print_stuff(FirstArg arg, Args ... args)
{
    std::lock_guard<std::mutex> lk(io_lock);
    std::cout << arg;
    print_stuff(args...);
}