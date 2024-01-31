 /**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer:       	 	   	      *
*	Date: 04/06/23	                  *
*	Subject: Simple Singleton    	  *
*				                	  *
**************************************/
#include <iostream>         // cout
#include <string>

#include "handleton.hpp"    // Handleton API
#include "factory_singleton.hpp"

namespace ilrd
{

class BasePlugin
{
public:
    virtual ~BasePlugin() = default;
    virtual void PluginPrint() {std::cout << "Base Pluging Print" << std::endl;}
};

} //namespace ilrd;