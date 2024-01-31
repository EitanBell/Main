/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Gal		              *
*	Date: 04/06/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Sinleton 				  *
*				                	  *
**************************************/	

#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <vector>
#include <cassert>

#include "shapes.hpp"
#include "factory.hpp"
#include "singleton.hpp"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define TEST_RES(status) \
			(status ? ANSI_COLOR_RED "\xe2\x9c\x97 FAIL" ANSI_COLOR_RESET: \
			ANSI_COLOR_GREEN "\xE2\x9C\x93 SUCCESS" ANSI_COLOR_RESET)

class SingletonTest
{
public:
    int GetX(){ return x; };
    int GetY(){ return y; };

private:
    int x;
    int y;
};

void ThreadRoutine(std::vector<SingletonTest *> *instances, 
                                        std::mutex *mutex, size_t numOfGetCalls)
{
    for (size_t i = 0; i < numOfGetCalls; ++i)
    {
        SingletonTest *instance = ilrd::Singleton<SingletonTest>::GetInstance();
        std::lock_guard<std::mutex> lock(*mutex);
        instances->emplace_back(instance);
    }
}

int main(void)
{
    static const size_t numOfThreads = 1000;
    static const size_t numOfGetCalls = 200;

    std::mutex mutex_sync;
    std::thread threads[numOfThreads];
    std::vector<SingletonTest *> instances;

    std::cout << "\n\t******************* TESTS BEGIN *******************\n";

    for (size_t i = 0; i < numOfThreads; ++i)
    {
        threads[i] = std::thread(&ThreadRoutine, &instances, 
                                                    &mutex_sync, numOfGetCalls);
    }

    for (size_t i = 0; i < numOfThreads; ++i)
    {
        threads[i].join();
    }

    size_t counter = 1;

    for (size_t i = 1; i < (numOfThreads * numOfGetCalls); ++i)
    {
        counter += (instances[0] == instances[i]);
    }

    assert((numOfGetCalls * numOfThreads) == counter);
    
    std::cout << "\n\t\t\tTest res: " << TEST_RES(0) << std::endl;
    std::cout << "\n\t***************** TESTS FINISHED *****************\n\n";

    return 0;
}

