/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: 	 	          	      *
*	Date: 24/05/23	                  *
*	Subject: Semaphore			      *
*				                	  *
**************************************/
#include "semaphore.hpp"
#include <iostream>
#include <thread>

using namespace ilrd;

void worker(Semaphore& semaphore, int id)
{
    semaphore.Wait();

    std::cout << "Worker " << id << " starts working." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Worker " << id << " finishes working." << std::endl;

    semaphore.Post();
}

int main(void)
{
    constexpr int NUM_WORKERS = 5;
    Semaphore semaphore(1);

    std::thread workers[NUM_WORKERS];

    for (int i = 0; i < NUM_WORKERS; ++i)
    {
        workers[i] = std::thread(worker, std::ref(semaphore), i + 1);
    }

    for (int i = 0; i < NUM_WORKERS; ++i)
    {
        workers[i].join();
    }

	std::cout << "SUCCESS"<< std::endl;

    return 0;
}