/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: 	                      *
*	Date: 01/06/23	                  *
*	Subject: Factory   	        	  *
*				                	  *
**************************************/

#ifndef ILRD_RD136_7_FACTORY_HPP
#define ILRD_RD136_7_FACTORY_HPP

#include <functional>       // std::function
#include <unordered_map>    // std::unordered_map    

namespace ilrd
{

template <typename BASE, typename KEY, typename... ARGS>
class Factory
{
public:
    void Add(KEY key, std::function<BASE *(ARGS...)> creator);
    BASE *Create(KEY key, ARGS... args);

private:
    std::unordered_map<KEY, std::function<BASE *(ARGS...)>> m_creators;
};

template <typename BASE, typename KEY, typename... ARGS>
void Factory<BASE, KEY, ARGS...>::Add(KEY key, 
                                         std::function<BASE *(ARGS...)> creator)
{
    m_creators[key] = creator;
}

template <typename BASE, typename KEY, typename... ARGS>
BASE *Factory<BASE, KEY, ARGS...>::Create(KEY key, ARGS ...args)
{
    return m_creators.at(key)(args...);
}

} //namespace ilrd

#endif /* ILRD_RD136_7_FACTORY_HPP */    

