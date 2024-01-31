 /**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Or     	 	   	      *
*	Date: 05/06/23	                  *
*	Subject: Handleton          	  *
*				                	  *
**************************************/
#ifndef __HANDLETON_HPP__
#define __HANDLETON_HPP__

#ifdef I_AM_THE_HANDLETON_IMPLEMENTER

#include <cstdlib>
#include <memory>
#include <mutex>
#include <atomic>

namespace ilrd
{

// T must have default private Ctor, in addition to having Handleton<T> as friend.
template<class T>
class Handleton
{
public:
    Handleton(const Handleton&) = delete;
    Handleton& operator=(const Handleton&) = delete;

    static T *GetInstance();

private:
    static std::mutex m_mutex;

    Handleton() = default;
};

template <typename T>
std::mutex Handleton<T>::m_mutex;

template <typename T>
T *Handleton<T>::GetInstance()
{
    static std::atomic<T *> m_instance(0);
    T *tmp = m_instance.load(std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_acquire);
    if (0 == tmp)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        tmp = m_instance.load(std::memory_order_relaxed);
        if (0 == tmp)
        {
            tmp = new T;
            std::atomic_thread_fence(std::memory_order_release);
            m_instance.store(tmp, std::memory_order_relaxed);

            std::atexit([]()
            {
                delete m_instance.load();
            });
        }
    }

    return tmp;
}

// Help the user with class instatiation syntax: 
//Google it "explicit template instantiation"
#define INSTANTIATE_HANDLETON(T) template class ilrd::Handleton<T>; 

} //namespace ilrd

#else

namespace ilrd
{
template<class T> 
class Handleton 
{
public:
	Handleton() = delete;
    Handleton(const Handleton&) = delete;
    Handleton& operator=(const Handleton&) = delete;

	static T *GetInstance();
};

} //namespace ilrd

#endif // I_AM_THE_HANDLETON_IMPLEMENTER

#endif // __HANDLETON_MODERN_HPP__


/* example usage

#define I_AM_THE_HANDLETON_IMPLEMENTER
#include "handleton_modern.hpp"

struct X
{
    int m_i;
};

// It should be in a .cpp file 
//implementing the single handleton instance:
INSTANTIATE_HANDLETON(X)
*/
 