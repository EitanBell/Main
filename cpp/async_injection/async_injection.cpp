/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Aviv		              *
*	Date: 20/06/23	                  *
*	Subject: async_injection		  *
*				                	  *
**************************************/		
#include "async_injection.hpp" // async_injection API
#define I_AM_THE_HANDLETON_IMPLEMENTER
#include "scheduler.hpp"       // scheduler API

namespace ilrd
{

/*********************** Implementation async_injection ***********************/

AsyncInjection::AsyncInjection(std::function<bool(void)> action, 
                          std::chrono::milliseconds interval) : m_action(action)
{
    std::shared_ptr<std::function<bool(void)>> 
    actionPtr(reinterpret_cast<std::function<bool(void)> *>(this),
    [](std::function<bool(void)> *ptr)
    {
        delete ptr;
    });

    Scheduler *sched = Handleton<Scheduler>::GetInstance();
    sched->AddTask(actionPtr, interval);
}

bool AsyncInjection::operator()()
{
    return m_action();
}

void AsyncInjection::Inject(std::function<bool(void)> action, 
                            std::chrono::milliseconds interval)
{   
    if (false == action())
    {
        new AsyncInjection(action, interval);
    }
}



}   // namespace ilrd