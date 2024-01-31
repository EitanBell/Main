/**************************************
*				   					  *
*	Developer: Shalom Bellaiche       *	 
*	Reviewer: 	 	          	      *
*	Date: 02/07/23	                  *
*	Project: Mail Manager System      *
*				                	  *
**************************************/
#ifndef MAIL_MANAGER_SYS_HPP
#define MAIL_MANAGER_SYS_HPP

#include <cstddef> 				// size_t
#include <mutex> 				// mutex
#include <condition_variable> 	// condition_variable
#include <chrono> 				// chrono_t

namespace brauda_college
{

class Semaphore
{
public:
    explicit Semaphore(std::size_t count = 0);
    Semaphore(const Semaphore&) = delete;
    Semaphore& operator=(const Semaphore&) = delete;

    void Post();
    void Wait();
    bool Wait(std::chrono::milliseconds timeout);

private:
    std::size_t m_count;
    std::mutex m_mutex;
    std::condition_variable m_condVar;
};

} //brauda_college

#endif // MAIL_MANAGER_SYS_HPP
