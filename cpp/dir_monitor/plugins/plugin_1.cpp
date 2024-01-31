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

class PluginDerived1: public PluginBase
{
public:    
    PluginDerived1() = default;
    virtual ~PluginDerived1() = default;

    void PluginPrint() {std::cout << "PluginDerived1 Print" << std::endl;}
};

__attribute__((constructor))
static void ShrdObjCtor()
{
    auto ptr = Handleton<FactorySingleton<PluginBase, std::string>>::GetInstance();
    cout << "Plugin no. 1 Factory Address:\t" << ptr << endl;

    ptr->Add("PluginDerived1", std::function<PluginBase *(void)>([]()
    {
        return (new PluginDerived1());
    }));
}

__attribute__((destructor))
static void ShrdObjDtor()
{
    auto ptr = Handleton<FactorySingleton<PluginBase, std::string>>::GetInstance();

    ptr->Remove("PluginDerived1");
}

}
