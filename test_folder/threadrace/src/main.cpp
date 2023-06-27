/**
 * @mainpage CS361 - Thread race 2000
 * @section Description
 * 
 * <Include some description here>
 * 
 * Make commands:
 * 
 *  make
 * 
 * will build the binary.
 * 
 *  make run
 * 
 * will run three races with 2, 4 and 10 racers.
 * 
 *  make clean
 * 
 * will clear out compiled code.
 * 
 *  make doc
 * 
 * will build the doxygen files.
 */

/**
 * @file
 * @author <Include your name here>
 * @date 2021-2022
 * @section Description
 * 
 * <Include description here>
*/

/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
#include <iostream>
#include <thread>
#include "semaphore.h"
#include "race.h"

extern int num_racers;
extern semaphore *sem;
extern std::queue<int> *dice_queue;
extern std::queue<std::thread::id> *win_queue;

int is_number(const char *input);

int main(int argc, char *argv[])
{
    int i;
    // semaphore *sem;
    std::thread *thread_pool;

    // Check user input
    if (argc != 2 || !is_number(argv[1])) {
        printf("Bad user!\n");
        exit(1);
    }

    // init stuff
    num_racers = atoi(argv[1]);
    sem = new semaphore(0);
    dice_queue = new std::queue<int>();
    win_queue = new std::queue<std::thread::id>();
    thread_pool = new std::thread[num_racers + 1];

    // init game master and racer threads
    thread_pool[0] = std::thread(game_master);
    for (i = 1; i <= num_racers; i++)
        thread_pool[i] = std::thread(racer);

    // wait for all threads to finish
    for (i = 0; i <= num_racers; i++)
        thread_pool[i].join();

    // print the winners
    for (i = 0; i < 3 && !win_queue->empty(); i++, win_queue->pop()) {
        std::cout << i + 1 << ": " << win_queue->front() << std::endl;
    }

    return 0;
}

int is_number(const char *input)
{
    const char *ptr = input;

    while (*ptr) {
        if (!isdigit(*ptr))
            return 0;
        
        ptr++;
    }

    return 1;
}