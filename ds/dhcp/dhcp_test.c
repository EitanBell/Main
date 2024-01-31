/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Gali	                  *
*	Date: 11/02/23	                  *
*	Valgrind & gdb tested             *
*	Subject: DHCP  	  				  *
*				                	  *
**************************************/

#include <stdio.h>	/* printf */
#include <string.h> /* strcmp */
	
#include "dhcp.h"	/* DHCP API */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")
#define SUBNET_MASK 4294967040 /* 11111111111111111111111100000000 */

enum ret_status
{
	FAIL = 1 
};

/*---- func declarations ---*/
void RunTests(void);
static int DHCPCreatAndDestroyTest(void);
static int DHCPAllocateIPTest(void);
static int DHCPFreeIPTest(void);
static int DHCPCountFreeAddressesTest(void);

/*---- main func ---*/
int main(void)
{
	RunTests();
	
	return 0;
}

void RunTests(void)
{
	printf("\n\t******************** TESTS BEGIN *******************\n\n");
	
	printf("\t\tDHCPCreate Test: \t\t%s\n", TEST_RES(DHCPCreatAndDestroyTest()));
	printf("\t\tDHCPAllocateIP Test: \t\t%s\n", TEST_RES(DHCPAllocateIPTest()));
	printf("\t\tDHCPFreeIP Test: \t\t%s\n", TEST_RES(DHCPFreeIPTest()));
	printf("\t\tDHCPCountFreeAddresses Test: \t%s\n", TEST_RES(DHCPCountFreeAddressesTest()));
		
	printf("\n\t****************** TESTS FINISHED ******************\n\n");
}

static int DHCPCreatAndDestroyTest(void)
{
	dhcp_t *dhcp = NULL;
	int status = 0;
	char address[100] = {0};
	const char *ip = "105.91.111.0";
	
	dhcp = DHCPCreate(ip, SUBNET_MASK);
	
	ip = "105.91.111.1";
	status += DHCPAllocateIP(dhcp, ip, address);
	ip = "105.91.111.2";
	status += DHCPAllocateIP(dhcp, ip, address);
	ip = "105.91.111.3";
	status += DHCPAllocateIP(dhcp, ip, address);
	ip = "105.91.111.4";
	status += DHCPAllocateIP(dhcp, ip, address);
	ip = "105.91.111.5";
	status += DHCPAllocateIP(dhcp, ip, address);
	ip = "105.91.111.6";
	status += DHCPAllocateIP(dhcp, ip, address);
	ip = "105.91.111.7";
	status += DHCPAllocateIP(dhcp, ip, address);
	ip = "105.91.111.8";
	status += DHCPAllocateIP(dhcp, ip, address);
	
	DHCPDestroy(dhcp);

	
	return status;
}

static int DHCPAllocateIPTest(void)
{
	const char *ip = "105.91.111.0";
	char out[50] = {0};
	status_t status = SUCCESS;
	dhcp_t *dhcp = DHCPCreate(ip, SUBNET_MASK);
	
	ip = "105.91.111.1";
	status += (SUCCESS == DHCPAllocateIP(dhcp, ip, out))? SUCCESS : FAIL;
	
	ip = "105.91.111.1";
	status += (OCCUPIED_ADDRESS == DHCPAllocateIP(dhcp, ip, out))? SUCCESS : FAIL;

	ip = "105.91.111.0";
	status += (OCCUPIED_ADDRESS == DHCPAllocateIP(dhcp, ip, out))? SUCCESS : FAIL;
	
	ip = "105.91.111.254";
	status += (NO_AVAIL_IP == DHCPAllocateIP(dhcp, ip, out))? SUCCESS : FAIL;
	
	ip = "105.91.111.255";
	status += (NO_AVAIL_IP == DHCPAllocateIP(dhcp, ip, out))? SUCCESS : FAIL;
		
	ip = "105.91.111.100";
	status += (SUCCESS == DHCPAllocateIP(dhcp, ip, out))? SUCCESS : FAIL;
	
	ip = "105.91.111.260";
	status += (INVALID_ADDRESS == DHCPAllocateIP(dhcp, ip, out))? SUCCESS : FAIL;
	
	ip = "105.91.111.253";
	status += (SUCCESS == DHCPAllocateIP(dhcp, ip, out))? SUCCESS : FAIL;
	
	ip = "105.91.111.253";
	status += (NO_AVAIL_IP == DHCPAllocateIP(dhcp, ip, out))? SUCCESS : FAIL;
	
	DHCPDestroy(dhcp);
	
	return status;
}

static int DHCPFreeIPTest(void)
{
	const char *ip = "105.91.111.0";
	char out[50] = {0};
	status_t status = SUCCESS;
	dhcp_t *dhcp = DHCPCreate(ip, SUBNET_MASK);
	
	ip = "105.91.111.1";
	status += (SUCCESS == DHCPAllocateIP(dhcp, ip, out))? SUCCESS : FAIL;
	
	status += (SUCCESS == DHCPFreeIP(dhcp, ip)? SUCCESS : FAIL);
	
	status += (DOUBLE_FREE == DHCPFreeIP(dhcp, ip)? SUCCESS : FAIL);

	status += (DOUBLE_FREE == DHCPFreeIP(dhcp, ip)? SUCCESS : FAIL);
	
	ip = "105.91.111.0";
	status += (SUCCESS == DHCPFreeIP(dhcp, ip)? SUCCESS : FAIL);
	
	ip = "105.91.111.254";
	status += (SUCCESS == DHCPFreeIP(dhcp, ip)? SUCCESS : FAIL);

	ip = "105.91.111.255";
	status += (SUCCESS == DHCPFreeIP(dhcp, ip)? SUCCESS : FAIL);
	
	ip = "105.91.111.100";
	status += (INVALID_ADDRESS == DHCPFreeIP(dhcp, ip)? SUCCESS : FAIL);
	
	ip = "105.91.111.100";
	status += (INVALID_ADDRESS == DHCPFreeIP(dhcp, ip)? SUCCESS : FAIL);
	
	ip = "105.91.111.260";
	status += (INVALID_ADDRESS == DHCPFreeIP(dhcp, ip)? SUCCESS : FAIL);
	
	DHCPDestroy(dhcp);
	
	return status;
}

static int DHCPCountFreeAddressesTest(void)
{
	const char *ip = "105.91.111.0";
	char out[50] = {0};
	size_t count = 253;
	int status = SUCCESS;
	
	dhcp_t *dhcp = DHCPCreate(ip, SUBNET_MASK);
	status += (count-- == DHCPCountFreeAddresses(dhcp)? SUCCESS : FAIL);
	
	DHCPAllocateIP(dhcp, ip, out);
	status += (count-- == DHCPCountFreeAddresses(dhcp)? SUCCESS : FAIL);
	
	DHCPAllocateIP(dhcp, ip, out);
	status += (count-- == DHCPCountFreeAddresses(dhcp)? SUCCESS : FAIL);
	
	DHCPAllocateIP(dhcp, ip, out);
	status += (count-- == DHCPCountFreeAddresses(dhcp)? SUCCESS : FAIL);
	
	DHCPAllocateIP(dhcp, ip, out);
	status += (count-- == DHCPCountFreeAddresses(dhcp)? SUCCESS : FAIL);
	
	DHCPAllocateIP(dhcp, ip, out);
	status += (count-- == DHCPCountFreeAddresses(dhcp)? SUCCESS : FAIL);
	
	DHCPAllocateIP(dhcp, ip, out);
	status += (count-- == DHCPCountFreeAddresses(dhcp)? SUCCESS : FAIL);
	
	DHCPAllocateIP(dhcp, ip, out);
	status += (count-- == DHCPCountFreeAddresses(dhcp)? SUCCESS : FAIL);
	
	DHCPAllocateIP(dhcp, ip, out);
	
	ip = "105.91.111.1";
	DHCPFreeIP(dhcp, ip);
	status += (++count == DHCPCountFreeAddresses(dhcp)? SUCCESS : FAIL);
	
	DHCPDestroy(dhcp);
	
	return status;
}

