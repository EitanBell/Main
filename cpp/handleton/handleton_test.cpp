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
#include <dlfcn.h>

#include "handleton.hpp"
#include "factory_singleton.hpp"
#include "plugin.hpp"

using namespace ilrd;

int Exercise_1(void);
int Exercise_2(void);
int Exercise_3(void);
void TryCreateDynObj(std::string key);

int main(void)
{
    Exercise_1();
    Exercise_2();
    Exercise_3();

    return 0;
}

int Exercise_1(void)
{
    FactorySingleton<BasePlugin, std::string> *factoryInstance = 
           Handleton<FactorySingleton<BasePlugin, std::string> >::GetInstance();

    std::cout << "Address of the FactorySingleton in test: " 
                                                << factoryInstance << std::endl;

    void *pluginHandle = dlopen("./libplugin.so", RTLD_LAZY);
    if (nullptr == pluginHandle)
    {
        std::cerr << "Failed to load plugin: " << dlerror() << std::endl;
        return 1;
    }

    void (*pluginFuncMain)() = reinterpret_cast<void (*)()>
                                          (dlsym(pluginHandle, "AddBase_main"));
    if (nullptr == pluginFuncMain)
    {
        std::cerr << "Failed to find symbol 'pluginFuncMain': " << dlerror()
                                                                   << std::endl;
        dlclose(pluginHandle);
        return 1;
    }

     void (*pluginFunc_1)() = reinterpret_cast<void (*)()>
                                          (dlsym(pluginHandle, "AddBase_1"));
    if (nullptr == pluginFunc_1)
    {
        std::cerr << "Failed to find symbol 'pluginFunc_1': " << dlerror()
                                                                   << std::endl;
        dlclose(pluginHandle);
        return 1;
    }

     void (*pluginFunc_2)() = reinterpret_cast<void (*)()>
                                          (dlsym(pluginHandle, "AddBase_2"));
    if (nullptr == pluginFunc_2)
    {
        std::cerr << "Failed to find symbol 'pluginFunc_2': " << dlerror()
                                                                   << std::endl;
        dlclose(pluginHandle);
        return 1;
    }

    pluginFuncMain();
    pluginFunc_1();
    pluginFunc_2();

    dlclose(pluginHandle);

    return 0;
}

int Exercise_2(void)
{
    FactorySingleton<BasePlugin, std::string> *factoryInstance = 
           Handleton<FactorySingleton<BasePlugin, std::string> >::GetInstance();

    BasePlugin *basePtr_1 = factoryInstance->Create("Base");
    basePtr_1->PluginPrint();

    BasePlugin *basePtr_2 = factoryInstance->Create("Plugin1");
    basePtr_2->PluginPrint();

    BasePlugin *basePtr_3 = factoryInstance->Create("Plugin2");
    basePtr_3->PluginPrint();

    delete basePtr_1;
    basePtr_1 = 0;

    delete basePtr_2;
    basePtr_2 = 0;

    delete basePtr_3;
    basePtr_3 = 0;

    return 0;
}

int Exercise_3(void)
{
    FactorySingleton<BasePlugin, std::string> *factoryInstance = 
           Handleton<FactorySingleton<BasePlugin, std::string> >::GetInstance();
    
    std::string buffer;
	std::cin >> buffer;

    std::cout << "factoryInstance address:\t" << factoryInstance << std::endl;

    while ("exit" != buffer)
    {
        void *so_handle = dlopen("./libplugin.so", RTLD_LAZY);
		if (nullptr == so_handle)
		{
			std::cerr << "Failed to load Shared Object" << dlerror() <<
                                                                      std::endl;
			return 1;
		}

        TryCreateDynObj(buffer);
		
		dlclose(so_handle);

		std::cin >> buffer;
    }

    return 0;
}

void TryCreateDynObj(std::string key)
{
    try
    {
        Handleton<FactorySingleton<BasePlugin, std::string>>::
                                                     GetInstance()->Create(key);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

