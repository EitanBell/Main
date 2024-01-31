/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: 	 	          	      *
*	Date: 27/05/23	                  *
*	Subject: waitable_queue		      *
*				                	  *
**************************************/
#ifndef ILRD_RD136_7_WAITABLE_QUEUE_HPP
#define ILRD_RD136_7_WAITABLE_QUEUE_HPP

#include "semaphore.hpp"

/*------------------------------ Documentation --------------------------------/

Waitalbe Queue is a thread safe priority queue.
Pop() is a blocking funtion if popping from an empty container - this is the reason for the timeout.
There ae requirements for the container provided by the user:
container should have the functions: push(), pop().

The type of the container used to store the elements must satisfy the 
requirements of Sequence Container. Additionally, it must provide the following 
functions with the usual semantics:
back(), front(), push_back(), pop_front().
The standard containers std::deque and std::list satisfy these requirements.
Please note(!)
	The function IsEmpty() is not thread safe, while the others are.
*/

namespace ilrd
{
template<class Container>
class WaitableQueue
{
public:
    explicit WaitableQueue();                            

    WaitableQueue(const WaitableQueue&) = delete;
    WaitableQueue& operator=(const WaitableQueue&) = delete;

    void Push(const typename Container::value_type& data);
    bool Pop(typename Container::reference buff);
    bool Pop(typename Container::reference buff, 
											 std::chrono::milliseconds timeout);
    bool IsEmpty() const;		//not thread safe

private:
    Container m_container;
    Semaphore m_sem;
    mutable std::mutex m_mutex;
};

template<class Container>
WaitableQueue<Container>::WaitableQueue() : m_sem(0) {}

template<class Container>
void WaitableQueue<Container>::Push(const typename Container::value_type& data)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_container.push(data);
    m_sem.Post();
}

template<class Container>
bool WaitableQueue<Container>::Pop(typename Container::reference buff)
{
    return (Pop(buff, std::chrono::hours(INT32_MAX)));
}

template<class Container>
bool WaitableQueue<Container>::Pop(typename Container::reference buff,
                                   			  std::chrono::milliseconds timeout)
{
    bool isTimeToPop = m_sem.Wait(timeout);

    if (isTimeToPop)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        buff = m_container.front();
        m_container.pop();
    }

    return isTimeToPop;
}

template<class Container>
bool WaitableQueue<Container>::IsEmpty() const
{
    return m_container.empty();
}

}//ilrd

#endif    // ILRD_RD136_7_WAITABLE_QUEUE_HPP  
