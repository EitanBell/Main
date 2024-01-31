/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Moriya         	      *
*	Date: 12/05/23	                  *
*	Subject: Scope Lock			      *
*				                	  *
**************************************/

#include <iostream>     			// ostream
#include <cstddef>      			// size_t
#include <mutex>					// mutex
#include <thread>					// thread

#include "scope_lock.hpp" // Scope Lock API 

using namespace ilrd;

void thread_func(size_t thread_num);

std::mutex g_mutex;

/*------------------------------ Scope Lock TEST -----------------------------*/

int main(void)
{
	size_t thread_num = 10;
	std::thread threads[10];
	
	for (size_t i = 0; thread_num > i; ++i)
    {
        threads[i] = std::thread(&thread_func, (i + 1));
    }
    
    for (size_t i = 0; thread_num > i; ++i)
    {
        threads[i].join();
    }

	return 0;
}

void thread_func(size_t thread_num)
{
     Scope_Lock<std::mutex> lock(g_mutex);
     
     std::cout << "Thread Number: " << thread_num << std::endl;
}

