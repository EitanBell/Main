 /**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Gal     	 	   	      *
*	Date: 04/06/23	                  *
*	Subject: Factory Singleton    	  *
*				                	  *
**************************************/

#ifndef ILRD_RD136_7_SIMPLE_SINGLETON_HPP
#define ILRD_RD136_7_SIMPLE_SINGLETON_HPP

#include <memory>           // std::shared_ptr
#include <unordered_map>    // std::unordered_map    
#include <functional>       // std::function

#include "factory.hpp"      // factory API
#include "handleton.hpp"    // handleton

namespace ilrd
{
    
/*----------------------------- FactorySingleton ------------------------------*/

template <typename BASE, typename KEY, typename... ARGS>
class FactorySingleton
{
public:
    void Add(KEY key, std::function<BASE *(ARGS...)> creator);
    BASE *Create(KEY key, ARGS... args);
    void Remove(KEY key);

private:
    FactorySingleton() = default;

    static std::shared_ptr<FactorySingleton> s_instance;
    std::unordered_map<KEY, std::function<BASE *(ARGS...)>> m_creators;

    friend Handleton<FactorySingleton<BASE, KEY, ARGS...>>;
};

template<class BASE, class KEY, class... ARGS> 
std::shared_ptr<FactorySingleton<BASE, KEY, ARGS...>> 
FactorySingleton<BASE, KEY, ARGS...>::s_instance(
new FactorySingleton<BASE, KEY, ARGS...>);

template<class BASE, class KEY, class... ARGS> 
void FactorySingleton<BASE, KEY, ARGS...>::Add(KEY key, 
                                         std::function<BASE *(ARGS...)> creator)
{
    m_creators[key] = creator;
}

template<class BASE, class KEY, class... ARGS> 
BASE *FactorySingleton<BASE, KEY, ARGS...>::Create(KEY key, ARGS ...args)
{
    try
    {
        return m_creators.at(key)(args...);
    }
    catch(const std::out_of_range& e)
    {
        throw std::invalid_argument("Key is not in the map");
    }
}

template <typename BASE, typename KEY, typename... ARGS>
void FactorySingleton<BASE, KEY,ARGS...>::Remove(KEY key)
{
    m_creators.erase(key);
}

}; //namespace ilrd

#endif /* ILRD_RD136_7_SIMPLE_SINGLETON_HPP */   
