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

#include "handleton.hpp"
#include "plugin.hpp"
#include "factory_singleton.hpp"

using namespace ilrd;

class Plugin2: public BasePlugin
{
public:    
    void PluginPrint() {std::cout << "Plugin2 Print" << std::endl;}
};

extern "C" FactorySingleton<BasePlugin, std::string> *AddBase_2(void)
{
    FactorySingleton<BasePlugin, std::string> *ptr = 
            Handleton<FactorySingleton<BasePlugin, std::string>>::GetInstance();

    std::cout << "Address of the FactorySingleton in plugin_2: " << 
                                                               ptr << std::endl;

    ptr->Add("Plugin2", std::function<BasePlugin *(void)>([]()
    {
        return (new Plugin2());
    }));
    
    return ptr;
}
