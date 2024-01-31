/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Date: 03/07/23	                  *
*	Subject: Framework plugins v0.3   *
**************************************/	

#include <iostream>             // cout
#include <mutex>                // mutex, lock_guard
#include <atomic>               // atomic
#include <chrono> 

#include "handleton.hpp"             // handleton API
#include "fw_plugins_v0.3.hpp"       // BaseCommand, PluginGuard template

namespace ilrd
{
    
class DerivedCommand1 : public BaseCommand
{
public:
    void Run(std::string data);
};

void DerivedCommand1::Run(std::string data)
{
    static int counter = 3;

    std::cout << data << std::endl;
    Framework<BaseCommand, std::string, std::string>::Async([]()
    {
        if (counter)
        {
            --counter;
            std::cout << "TEST RERUN COUNTER: " << counter << std::endl;
            return false;
        }
        
        return true;
    }, std::chrono::seconds(3));
}

static PluginGuard<DerivedCommand1> s_derivedCmd1("Ping");

} // namespace ilrd
