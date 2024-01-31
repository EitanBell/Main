/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Date: 14/07/23                    *
*	Subject: Framework commands       *
**************************************/
#ifndef __FW_COMMANDS_HPP__
#define __FW_COMMANDS_HPP__

#include <linux/nbd.h>        // nbd request/reply
#include <netinet/in.h>       // ntohl (byte order)

#include "logger.hpp"
#include "factory_singleton.hpp"
#include "framework_v0.4.hpp"       

namespace ilrd
{

struct NbdData;

class BaseCommand
{
public:
    virtual ~BaseCommand() = default;
    virtual void Run(NbdData *data) =0;
};

typedef FactorySingleton<BaseCommand, std::string, NbdData *> factoryBase_t;

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
PluginGuard<T>::PluginGuard(std::string key) : m_key(key)
{
    auto cmdFact = Handleton<factoryBase_t>::GetInstance();
    cmdFact->Add(key, [](NbdData *data)
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
#endif // __FW_COMMANDS_HPP__
