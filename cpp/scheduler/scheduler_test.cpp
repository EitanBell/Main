/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Gal		              *
*	Date: 18/06/23	                  *
*	Subject: scheduler		   		  *
*				                	  *
**************************************/		

#include <iostream>		// cout
#include <algorithm>

#define I_AM_THE_HANDLETON_IMPLEMENTER
#include "handleton.hpp"
#include "scheduler.hpp"		

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define TEST_RES(status) \
			(status ? ANSI_COLOR_RED "\xe2\x9c\x97 FAIL" ANSI_COLOR_RESET: \
			ANSI_COLOR_GREEN "\xE2\x9C\x93 SUCCESS" ANSI_COLOR_RESET)


static void RunTests(void);

int main(void)
{
	RunTests();
	
	return 0;
}

static void RunTests(void)
{
	ilrd::Scheduler *sched = ilrd::Handleton<ilrd::Scheduler>::GetInstance();
    sched->Run();
    std::vector<int> priority;

	bool isUpdated = false;

    sched->AddTask(std::make_shared<std::function<bool(void)>>
                   (std::function<bool(void)>([&isUpdated]()
    {
        isUpdated = true;
        return true;

    })), std::chrono::seconds(1));

    sched->Stop();

	std::cout << "Test Results: " << TEST_RES(!(false == isUpdated)) << std::endl;

    sched->Run();

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Test Results: " << TEST_RES(!(true == isUpdated)) << std::endl;

    sched->AddTask(std::make_shared<std::function<bool(void)>>
                   (std::function<bool(void)>([&priority]()
    {
        priority.push_back(2);
        return true;
    })), std::chrono::milliseconds(300));

    sched->AddTask(std::make_shared<std::function<bool(void)>>
                   (std::function<bool(void)>([&priority]()
    {
        priority.push_back(3);
        return true;
    })), std::chrono::milliseconds(500));

    sched->AddTask(std::make_shared<std::function<bool(void)>>
                   (std::function<bool(void)>([&priority]()
    {
        priority.push_back(1);
        return true;
    })), std::chrono::milliseconds(0));

	sched->AddTask(std::make_shared<std::function<bool(void)>>
                   (std::function<bool(void)>([&priority]()
    {
        priority.push_back(5);
        return true;
    })), std::chrono::milliseconds(800));

	sched->AddTask(std::make_shared<std::function<bool(void)>>
                   (std::function<bool(void)>([&priority]()
    {
        priority.push_back(4);
        return true;
    })), std::chrono::milliseconds(600));

    std::this_thread::sleep_for(std::chrono::seconds(2));

	for (auto iter : priority)
	{
		std::cout << iter << std::endl;
	}

	std::cout << "Test Results: " << TEST_RES
			(!std::is_sorted(priority.begin(), priority.end())) << std::endl;
}
