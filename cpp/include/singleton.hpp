 /**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer:       	 	   	      *
*	Date: 04/06/23	                  *
*	Subject: Simple Singleton    	  *
*				                	  *
**************************************/

#ifndef ILRD_RD136_7_SINGLETON_HPP
#define ILRD_RD136_7_SINGLETON_HPP

#include <atomic>   // atomic
#include <mutex>    // mutex

namespace ilrd
{

/*------------------------------- Singleton ----------------------------------*/

template <typename T>
class Singleton
{
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static T *GetInstance();

private:
    static std::mutex m_mutex;

    Singleton() = default;
};

template <typename T>
std::mutex Singleton<T>::m_mutex;

template <typename T>
T *Singleton<T>::GetInstance()
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

}; //namespace ilrd

#endif /* ILRD_RD136_7_SINGLETON_HPP */   