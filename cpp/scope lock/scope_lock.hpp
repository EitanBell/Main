/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Moriya         	      *
*	Date: 12/05/23	                  *
*	Subject: Scope Lock			      *
*				                	  *
**************************************/

#ifndef ILRD_RD136_7_SCOPE_LOCK_HPP
#define ILRD_RD136_7_SCOPE_LOCK_HPP

#include <boost/noncopyable.hpp>		// boost::noncopyable

namespace ilrd
{
template <typename T>		// T should be lockable obj type.
class Scope_Lock : private boost::noncopyable
{
public:
	explicit Scope_Lock(T& lockable);
	~Scope_Lock();

private:
	T& m_lock;
};

template <typename T>		// T should be lockable obj type.
Scope_Lock<T>::Scope_Lock(T& lockable): m_lock(lockable)
{
	m_lock.lock();
	#ifndef NDEBUG
		std::cout << "Scope_Lock: locked" << std::endl;
    #endif
}

template <typename T>		// T should be lockable obj type.
Scope_Lock<T>::~Scope_Lock()
{
	m_lock.unlock();
	#ifndef NDEBUG
		std::cout << "Scope_Lock: unlocked" << std::endl << std::endl;
	#endif
}

}//namespace ilrd

#endif //ILRD_RD136_7_SCOPE_LOCK_HPP

