/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: 	 	          	      *
*	Date: 27/05/23	                  *
*	Subject: waitable_queue		      *
*				                	  *
**************************************/
#include <iostream>
#include <vector>
#include <future>
#include <cassert>
#include <unistd.h>

#include "priority_queue.hpp"
#include "waitable_queue.hpp"
	
/*----------------------------------------------------------------------------*/
using namespace ilrd;
using namespace std;

static void RunTests(void);

int main(void)
{
	RunTests();
	
	return 0;
}

static void RunTests(void)
{
	std::vector<std::future<void>> futures_push;
	std::vector<std::future<void>> futures_pop;
	ilrd::WaitableQueue<ilrd::PriorityQueue<int>> queue;
	std::chrono::milliseconds deadline(5000);
	int counter = 0;

	(queue.IsEmpty()) ? std::cout << "IsEmpty() Success ✅\n" : 
											 std::cout << "IsEmpty() Fail ❌\n";

	(false == queue.Pop(counter, deadline)) ? std::cout << "Pop () Success ✅\n" : 
											  std::cout << "Pop() Fail ❌\n";

	for (int i = 0; 100 > i; ++i)
    {
        futures_push.push_back(std::async(std::launch::async, [&counter, &queue, i]
        {
			queue.Push(i);
			++counter;
        }));

		futures_pop.push_back(std::async(std::launch::async, [&counter, &queue, i]
        {
			int temp = 0;
			queue.Pop(temp);

			--counter;
        }));
    }

	for (auto &fut : futures_push)
    {
        fut.wait();
    }
	for (auto &fut : futures_pop)
    {
        fut.wait();
    }
	
	(0 == counter) ? std::cout << "Push() Success ✅\n" : std::cout << "Push() Fail ❌\n";
	(queue.IsEmpty()) ? std::cout << "TEST Success ✅\n" : std::cout << "TEST Fail ❌\n";
}

