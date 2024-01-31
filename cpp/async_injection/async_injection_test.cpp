/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Aviv		              *
*	Date: 20/06/23	                  *
*	Subject: async_injection		  *
*				                	  *
**************************************/	
#include <iostream>
#include <thread>

#include "async_injection.hpp"

void TestActionReturnsFailMoreThanMaxLimit(void);
void TestActionReturnsTrueInFirstRun(void);

int main(void)
{
    std::cout << "Testing async_injection" << std::endl;

    std::cout << "Testing action that returns fail more than the max limit:" << std::endl;
    TestActionReturnsFailMoreThanMaxLimit();

    std::cout << "Testing action that returns true in the first run:" << std::endl;
    TestActionReturnsTrueInFirstRun();

    return 0;
}

void TestActionReturnsFailMoreThanMaxLimit(void)
{
    std::function<bool(void)> action = []() {
        static int counter = 0;
        bool status = false;

        if (3 == counter)
        {
            std::cout << "Got Max limit of running action - Stopped running" << std::endl;
            status = true;	
        }
        else
        {
            std::cout << "Action execution #" << counter + 1 << std::endl;
        }

        ++counter;
        return status;
    };

    std::chrono::milliseconds interval(100); 
    ilrd::AsyncInjection::Inject(action, interval);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void TestActionReturnsTrueInFirstRun(void)
{
    std::function<bool(void)> action = []() {
        static bool firstRun = true;
        if (firstRun)
        {
            firstRun = false;
            std::cout << "Action execution #1" << std::endl;
            return true;
        }
        else
        {
            std::cout << "Should not execute this" << std::endl;
            return false;
        }
    };

    std::chrono::milliseconds interval(500);
    ilrd::AsyncInjection::Inject(action, interval);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

