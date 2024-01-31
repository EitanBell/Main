/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer:   		              *
*	Date: 04/06/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Handleton 				  *
*				                	  *
**************************************/	

#include <iostream>
#include <functional>

#define I_AM_THE_HANDLETON_IMPLEMENTER
#include "handleton.hpp"

#include "factory_singleton.hpp"
#include "plugin.hpp"

using namespace ilrd;

extern "C" FactorySingleton<BasePlugin, std::string> *AddBase_main(void)
{
    FactorySingleton<BasePlugin, std::string> *ptr = 
            Handleton<FactorySingleton<BasePlugin, std::string>>::GetInstance();
    
    std::cout << "Address of the FactorySingleton in main: " << ptr << std::endl;

    ptr->Add("Base", std::function<BasePlugin *(void)>([]()
    {
        return (new BasePlugin());
    }));
    
    return ptr;
}
