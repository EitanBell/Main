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

class PluginDerived2: public PluginBase
{
public:    
    PluginDerived2() = default;
    virtual ~PluginDerived2() = default;

    void PluginPrint() {std::cout << "PluginDerived2 Print" << std::endl;}
};

class ShrdObjWrapper
{
public:
    ShrdObjWrapper();
    ~ShrdObjWrapper();
};

__attribute__((constructor))
static void ShrdObjCtor()
{
    auto ptr = Handleton<FactorySingleton<PluginBase, std::string>>::GetInstance();
    cout << "Plugin no. 2 Factory Address:\t" << ptr << endl;

    ptr->Add("PluginDerived2", std::function<PluginBase *(void)>([]()
    {
        return (new PluginDerived2());
    }));
}

__attribute__((destructor))
static void ShrdObjDtor()
{
    auto ptr = Handleton<FactorySingleton<PluginBase, std::string>>::GetInstance();

    ptr->Remove("PluginDerived2");
}

}
