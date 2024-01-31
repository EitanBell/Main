/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer:   		              *
*	Date: 04/06/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Handleton 				  *
*				                	  *
**************************************/	
#define I_AM_THE_HANDLETON_IMPLEMENTER

using namespace ilrd;

#include <iostream>

#include "singleton.hpp"
#include "handleton.hpp"

class ExampleClass
{
public:

private:
    ExampleClass() = default;

    friend Singleton<ExampleClass>;
    friend Handleton<ExampleClass>;
};


extern "C" ExampleClass *PluginFunc_1(void)
{
    ExampleClass *pluginInstance = Singleton<ExampleClass>::GetInstance();

    return pluginInstance;
}

extern "C" ExampleClass *PluginFunc_2(void)
{
    ExampleClass *pluginInstance = Handleton<ExampleClass>::GetInstance();

    return pluginInstance;
}

extern "C" ExampleClass *PluginFunc_3(void)
{
    ExampleClass *pluginInstance = Handleton<ExampleClass>::GetInstance();

    return pluginInstance;
}
