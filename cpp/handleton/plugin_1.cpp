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

class Plugin1: public BasePlugin
{
public:    
    Plugin1() = default;
    virtual ~Plugin1() = default;
    
    void PluginPrint() {std::cout << "Plugin1 Print" << std::endl;}
};

extern "C" FactorySingleton<BasePlugin, std::string> *AddBase_1(void)
{
    FactorySingleton<BasePlugin, std::string> *ptr = 
            Handleton<FactorySingleton<BasePlugin, std::string>>::GetInstance();

    std::cout << "Address of the FactorySingleton in plugin_1: " << 
                                                               ptr << std::endl;

    ptr->Add("Plugin1", std::function<BasePlugin *(void)>([]()
    {
        return (new Plugin1());
    }));
    
    return ptr;
}
