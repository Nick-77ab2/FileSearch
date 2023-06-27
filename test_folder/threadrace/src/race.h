#ifndef __RACE_H__
#define __RACE_H__

#include <mutex>
#include <condition_variable>
#include <iostream>
#include <queue>
#include <thread>
#include <string>

#include "semaphore.h"

// thread function for game master
void game_master();
// thread function for racer
void racer();

#endif