/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Gali	                  *
*	Date: 11/02/23	                  *
*	Valgrind & gdb tested             *
*	Subject: DHCP  	  				  *
*				                	  *
**************************************/

#ifndef __DHCP_H__
#define __DHCP_H__

#include <stddef.h> /* size_t */

/*------------------------------------------------------------------------------
General Description|
-------------------
	Dynamic Host Configuration Protocol (DHCP) is a network management protocol 
	used to allocate IP addresses for devices in a local network (LAN).
Attributes
	Unique IP addresses
Main Functions
	Allocate IP, Free IP
Other Functions
	Count free addresses
------------------------------------------------------------------------------*/

typedef struct dhcp dhcp_t;

typedef enum status
{
    SYS_FAULT = -1,
    SUCCESS = 0,
    INVALID_ADDRESS = 1,
    DOUBLE_FREE = 2,
    OCCUPIED_ADDRESS = 3,
    NO_AVAIL_IP = 4	
} status_t;

/*-----------------------------------------------------------------------------
Description:
	Create an DHCP handler for IP addresses allocation.
Parameters:
	net_ip - The network IP to be managed by the DHCP.
	subnet_mask - A mask defining the size and range of addresses to be
				  managed by the DHCP for the network.
Return Value:
	DHCP handler
NOTE!
	Passing invalid subnet_mask argument may cause an undifined behavior.
Complexity: 
	time: O(log(n))  
-----------------------------------------------------------------------------*/
dhcp_t *DHCPCreate(const char *net_ip, unsigned int subnet_mask);

/*-----------------------------------------------------------------------------
Description:
	Destroy all resources created for the DHCP.
Parameters:
	'dhcp' - The DHCP to be destroyed.
Return Value:
	NON.
Complexity: 
	time: O(n) 
-----------------------------------------------------------------------------*/
void DHCPDestroy(dhcp_t *dhcp);

/*-----------------------------------------------------------------------------
Description:
	Allocate a unique IP address for new network user.
Parameters:
	dhcp - A handle to the dhcp.
	requested_ip - A requested specific IP provided by the user.
	res_ip - The allocated IP address.
Return Value:
	 -1 - Failed to allocate an IP address.
	  0 - successfully allocate an IP address.
	  1 - If the requested address is not valid, the  allocated address is the
		  smallest free address.
	  3 - The requested address is already allocated. The next smallest 
		  address will be allocated.
	  4 - If all the bigger addresses than the requested_ip are already
		  allocated.
Complexity: 
	time: Avg - O(log(n)), Worst - O(nlog(n)).  
-----------------------------------------------------------------------------*/
status_t DHCPAllocateIP(dhcp_t *dhcp, const char *requested_ip, char *res_ip);

/*-----------------------------------------------------------------------------
Description:
	Free an allocated IP address.
Parameters:
	dhcp - A handle to the dhcp.
	ip - The IP address(IPV4) to free.
Return Value:
	 -1 - Failed to free an IP address.
	  0 - IP address successfully freed.
	  2 - IP address has already been freed.

Complexity: 
	time: O(log(n))
-----------------------------------------------------------------------------*/
status_t DHCPFreeIP(dhcp_t *dhcp, const char *ip);

/*-----------------------------------------------------------------------------
Description:
	Counts the available IP addresses.
Parameters:
	dhcp - A handle to the dhcp.
Return Value:
	The amount of available IP addresses.
Complexity: 
	time: O(n) 
-----------------------------------------------------------------------------*/
size_t DHCPCountFreeAddresses(const dhcp_t *dhcp);

#endif /* __DHCP_H__ */

