/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Reviewer:  Eliran                 *
*	Date: 03/07/23	                  *
*	Subject: Framework plugins v0.2   *
**************************************/	

#include <iostream>             // cout
#include <mutex>                // mutex, lock_guard
#include <atomic>               // atomic

#include "handleton.hpp"        // handleton
#include "fw_plugins_v0.2.hpp"    // BaseCommand, PluginGuard template

namespace ilrd
{
    
class DerivedCommand2 : public BaseCommand
{
public:
    void Run(std::string data);
};

void DerivedCommand2::Run(std::string data)
{
    std::cout << data << std::endl;
}
static PluginGuard<DerivedCommand2> s_derivedCmd2("hi");

} // namespace ilrd