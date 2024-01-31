/**************************************
*   Developer: Eitan Bellaiche        *	 
*	Date: 03/07/23                    *
*   Reviewer: Eliran                  *
*	Subject: Framework v0.2 		  *
**************************************/	
#define I_AM_THE_HANDLETON_IMPLEMENTER

#include "framework_v0.2.hpp"
#include "fw_plugins_v0.2.hpp"

namespace ilrd
{
    
INSTANTIATE_HANDLETON(factoryBase_t);
INSTANTIATE_HANDLETON(Logger);

} // namespace ilrd
