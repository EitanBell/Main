/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Date: 03/07/23                    *
*	Subject: Framework plugins v0.3   *
**************************************/
#ifndef __FW_PLUGINS_HPP__
#define __FW_PLUGINS_HPP__

#include "logger.hpp"
#include "factory_singleton.hpp"
#include "framework_v0.3.hpp"

namespace ilrd
{

class BaseCommand
{
public:
    virtual ~BaseCommand(){}
    virtual void Run(std::string data) =0;
};

typedef FactorySingleton<BaseCommand, std::string, std::string> factoryBase_t;

template <class T>
class PluginGuard
{
public:
    PluginGuard(std::string key);
    ~PluginGuard();

private:
    std::string m_key;
};

template<class T> 
PluginGuard<T>::PluginGuard(std::string key) :
    m_key(key)
{
    auto cmdFact = Handleton<factoryBase_t>::GetInstance();
    cmdFact->Add(key, [](std::string data)
    {
        (void)data;
        return (new T);
    });

    Handleton<Logger>::GetInstance()->Log(
            "Added plugin from command factory --> " + m_key + "\n");
}

template<class T> 
PluginGuard<T>::~PluginGuard()
{
    auto cmdFact = Handleton<factoryBase_t>::GetInstance();
    Handleton<Logger>::GetInstance()->Log(
            "Removed plugin from command factory --> " + m_key + "\n");
    cmdFact->Remove(m_key);
}

}


#endif // __FW_PLUGINS_HPP__
