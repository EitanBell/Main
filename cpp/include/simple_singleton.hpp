 /**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer:       	 	   	      *
*	Date: 04/06/23	                  *
*	Subject: Simple Singleton    	  *
*				                	  *
**************************************/

#ifndef ILRD_RD136_7_SIMPLE_SINGLETON_HPP
#define ILRD_RD136_7_SIMPLE_SINGLETON_HPP

#include <memory>   //unique_ptr

#include "factory.hpp"      // factory API

namespace ilrd
{

/*----------------------------- SimpleSingleton ------------------------------*/

class SimpleSingleton
{
public:
    static SimpleSingleton *GetInstance();
    SimpleSingleton(const SimpleSingleton&) = delete;
    SimpleSingleton& operator=(const SimpleSingleton&) = delete;

private:
    SimpleSingleton();
    static std::unique_ptr<SimpleSingleton> s_instance;
};

std::unique_ptr<SimpleSingleton> 
SimpleSingleton::s_instance(new SimpleSingleton);

SimpleSingleton::SimpleSingleton() {}

SimpleSingleton *SimpleSingleton::GetInstance()
{
    return s_instance.get();
}

}; //namespace ilrd

#endif /* ILRD_RD136_7_SIMPLE_SINGLETON_HPP */   
