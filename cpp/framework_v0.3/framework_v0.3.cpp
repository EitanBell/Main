/**************************************
*   Developer: Eitan Bellaiche        *	 
*	Date: 03/07/23                    *
*	Subject: Framework v0.3 		  *
**************************************/	
#define I_AM_THE_HANDLETON_IMPLEMENTER

#include "framework_v0.3.hpp"
#include "fw_plugins_v0.3.hpp"
#include "scheduler.hpp"

namespace ilrd
{
    
INSTANTIATE_HANDLETON(factoryBase_t);
INSTANTIATE_HANDLETON(Logger);
INSTANTIATE_HANDLETON(Scheduler);

template class Framework<BaseCommand, std::string, std::string>;

} // namespace ilrd
