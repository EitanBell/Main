/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Aviv		              *
*	Date: 20/06/23	                  *
*	Subject: async_injection		  *
*				                	  *
**************************************/		

#ifndef ILRD_RD136_7_ASYNC_INJECTION_HPP
#define ILRD_RD136_7_ASYNC_INJECTION_HPP

#include <memory>       // shared_ptr
#include <functional>   // function
#include <chrono>       // milliseconds

namespace ilrd
{

class AsyncInjection
{
public:
    AsyncInjection(const AsyncInjection&) = delete;
    AsyncInjection& operator=(const AsyncInjection&) = delete;
    
    static void Inject(std::function<bool(void)> action, 
                                            std::chrono::milliseconds interval);
private:
    AsyncInjection(std::function<bool(void)> action, 
                                            std::chrono::milliseconds interval);
    ~AsyncInjection() = default;

    bool operator()();

    std::function<bool(void)> m_action;
};

}   // namespace ilrd


#endif /* ILRD_RD136_7_ASYNC_INJECTION_HPP */