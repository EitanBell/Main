/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: 	 	          	      *
*	Date: 24/05/23	                  *
*	Subject: Semaphore			      *
*				                	  *
**************************************/
#include "semaphore.hpp" // semaphore API

namespace ilrd
{
Semaphore::Semaphore(std::size_t count): m_count(count) {}

void Semaphore::Post()
{
	std::unique_lock<std::mutex> lock(m_mutex);

	++m_count;
	m_condVar.notify_one(); 
}

void Semaphore::Wait()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_condVar.wait(lock, [this] {return (0 < m_count);});
	
	--m_count;
}

bool Semaphore::Wait(std::chrono::milliseconds timeout)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    bool ret_val = m_condVar.wait_for(lock, timeout, 
												 [this]{return (0 < m_count);});
    
	if (false != ret_val)
    {
        --m_count;
    }

    return ret_val;
}

} //namespace ilrd
