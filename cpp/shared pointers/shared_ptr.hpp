/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Ariel         	      *
*	Date: 14/05/23	                  *
*	Subject: Shared Pointers	      *
*				                	  *
**************************************/
#ifndef ILRD_RD136_7_SHARED_PTR_HPP
#define ILRD_RD136_7_SHARED_PTR_HPP

#include <iostream>  

namespace ilrd
{
template <typename T>
class SharedPtr
{
public:
    explicit SharedPtr(T *ptr = 0);
    SharedPtr(const SharedPtr& other);
    
    template <typename S>
    SharedPtr(const SharedPtr<S>& other);
    ~SharedPtr();

    SharedPtr& operator=(const SharedPtr& other);
    T& operator*();
    T *operator->();

private:
    T *m_ptr;
    std::size_t *m_counter;
    
    void DeletePtr();
    
    template <typename S>
    friend class SharedPtr;
};

template <typename T>
SharedPtr<T>::SharedPtr(T *ptr): m_ptr(ptr), m_counter(new std::size_t(1)){}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& other): m_ptr(other.m_ptr),
                                                 	  m_counter(other.m_counter)
{
    ++(*m_counter);
}

template<typename T> 
template<typename S> 
SharedPtr<T>::SharedPtr(const SharedPtr<S> &other): 
							      m_ptr(other.m_ptr), m_counter(other.m_counter) 
{
    ++(*m_counter);
}


template<class T>
SharedPtr<T>::~SharedPtr()
{
    DeletePtr();
}

template <typename T>
void SharedPtr<T>::DeletePtr()
{
    if (1 == (*m_counter))
    {
        delete m_ptr;
        m_ptr = 0;
        delete m_counter;
        m_counter = 0;
    }
    else
    {
        --(*m_counter);
    }
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& other)
{
    ++(*other.m_counter);	//Self Assignment is considered. Saves call to "if";
    DeletePtr();
    m_ptr = other.m_ptr;
    m_counter = other.m_counter;

    return *this;
}

template <typename T>
T& SharedPtr<T>::operator*()
{
    return *m_ptr;
}

template <typename T>
T *SharedPtr<T>::operator->()
{
    return m_ptr;
}

}//namespace ilrd

#endif //ILRD_RD136_7_SHARED_PTR_HPP

