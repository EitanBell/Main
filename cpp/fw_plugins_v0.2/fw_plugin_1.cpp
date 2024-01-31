/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Reviewer:  Eliran                 *
*	Date: 03/07/23	                  *
*	Subject: Framework plugins v0.2   *
**************************************/	

#include <iostream>             // cout
#include <mutex>                // mutex, lock_guard
#include <atomic>               // atomic
#include <chrono> 

#include "fw_plugins_v0.2.hpp"       // BaseCommand, PluginGuard template

namespace ilrd
{
    
class DerivedCommand1 : public BaseCommand
{
public:
    void Run(std::string data);
};

void DerivedCommand1::Run(std::string data)
{
    std::cout << data << std::endl;
}

static PluginGuard<DerivedCommand1> s_derivedCmd1("Ping");

} // namespace ilrd
