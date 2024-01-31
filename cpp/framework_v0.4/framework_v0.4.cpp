/**************************************
*   Developer: Eitan Bellaiche        *	 
*	Date: 14/07/23                    *
*	Subject: Framework v0.4 		  *
**************************************/	
#define I_AM_THE_HANDLETON_IMPLEMENTER

#include "framework_v0.4.hpp"
#include "fw_commands.hpp"
#include "scheduler.hpp"

namespace ilrd
{
    
INSTANTIATE_HANDLETON(factoryBase_t);
INSTANTIATE_HANDLETON(Logger);
INSTANTIATE_HANDLETON(Scheduler);

template class Framework<BaseCommand, std::string, NbdData *>;

} // namespace ilrd
