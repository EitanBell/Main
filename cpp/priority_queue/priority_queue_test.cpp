/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: 	                      *
*	Date: 24/05/23	                  *
*	Subject: Priority Queue   		  *
*				                	  *
**************************************/			

#define TEST_RES(status) (status ? "\xe2\x9c\x97\033[0;31m  FAIL\033[0m" : "\xE2\x9C\x93 \033[0;32m SUCCESS\033[0m")
#define BOOL_TO_STATUS(bool) ((bool) ^ 1)
	   			        
#include <iostream>		// cout

#include "priority_queue.hpp" // priority_queue API 

static void Tests(void);

int main(void)
{
	Tests();

	return 0;
}

static void Tests(void)
{
	using namespace ilrd;

	PriorityQueue<int> pq;
	int status = 0;

	std::cout << "\n\t******************** TESTS BEGIN *******************\n\n";
		
	std::cout << "\t\tempty()       \tTest:   " 
				<< TEST_RES(BOOL_TO_STATUS(pq.empty()));
	
	for (std::size_t i = 0; 1000 > i; ++i)
	{
		int random = (rand() % 1000);
		pq.push(random);
		status += (pq.Front() >= random);
	}

	std::cout << "\n\t\tpush()		Test:   " 
				<< TEST_RES(BOOL_TO_STATUS((1000 == status) && !(pq.empty())));
	std::cout << "\n\t\tFront() 	Test:   " 
				<< TEST_RES(BOOL_TO_STATUS((1000 == status) && !(pq.empty())));

	status = 0;
	for (std::size_t i = 0; 1000 > i; ++i)
	{
		int popped = pq.Front();
		pq.pop();
		status += (popped >= pq.Front());
	}

	std::cout << "\n\t\tpop()          \tTest:   " 
				<< TEST_RES(BOOL_TO_STATUS((1000 == status) && (pq.empty())));
							
	std::cout << "\n\n\t****************** TESTS FINISHED ******************\n\n";
}


