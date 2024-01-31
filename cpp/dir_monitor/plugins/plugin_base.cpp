#include <string>
#include <functional>
#include <iostream>

#include "handleton.hpp"    // Handleton
#include "plugin_main.hpp"
#include "factory_singleton.hpp"

using namespace ilrd;
using std::cout;
using std::endl;

namespace ilrd
{

__attribute__((constructor))
static void ShrdObjCtor()
{
    auto ptr = Handleton<FactorySingleton<PluginBase, std::string>>::GetInstance();
    cout << "Main Plugin Factory Address:\t" << ptr << endl;

    ptr->Add("PluginBase", std::function<PluginBase *(void)>([]()
    {
        return (new PluginBase());
    }));
}

__attribute__((destructor))
static void ShrdObjDtor()
{
    auto ptr = Handleton<FactorySingleton<PluginBase, std::string>>::GetInstance();

    ptr->Remove("PluginBase");
}

}

