/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Gali	                  *
*	Date: 11/02/23	                  *
*	Valgrind & gdb tested             *
*	Subject: DHCP  	  				  *
*				                	  *
**************************************/

#include <assert.h>  	/* assert */
#include <stdlib.h> 	/* malloc, free */
#include <arpa/inet.h>	/* inet_pton */
#include <netinet/in.h>	/* struct in_addr */
#include <string.h>		/* strlen */

#include "dhcp.h"	/* DHCP API */

#define ZERO_ADDRESS (1)

enum ret_status
{
	FAIL = 1 
};

typedef enum bit
{
	OFF = 0,
	ON,
	BIT_SIZE
} bit_t;

typedef enum bool_status
{
	FALSE = 0,
	TRUE
} bool_status_t;

typedef struct node_trie
{
	struct node_trie *child[BIT_SIZE];	
	bool_status_t is_allocated;
} node_trie_t;

struct dhcp
{
	node_trie_t *root;
	char *net_ip;
	unsigned int subnet_mask;
	size_t trie_depth;
};

/*------------- Helper Funcs -------------*/
static size_t CountSetBits(unsigned int num);
static void NodeInit(node_trie_t *node);
static node_trie_t *CreateNode(void);
static int DHCPInit(dhcp_t *new_dhcp, const char *net_ip,
													  unsigned int subnet_mask);
static int CreateAddress(dhcp_t *dhcp, unsigned int *ip);
static void DestroyNodes(node_trie_t *node);
static size_t GetAllocatedSize(node_trie_t *node);
							
static int IsIPBelongsToNetwork(dhcp_t *dhcp, const char *requested_ip);													   
/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

/*------------------------------ DHCPCreate ----------------------------------*/
dhcp_t *DHCPCreate(const char *net_ip, unsigned int subnet_mask)
{
	dhcp_t *new_dhcp = NULL;

	assert(NULL != net_ip);
	
	new_dhcp = (dhcp_t *)malloc(sizeof(dhcp_t));
	if (NULL == new_dhcp)
	{
		return NULL;
	}
	
	if (SYS_FAULT == DHCPInit(new_dhcp, net_ip, subnet_mask))
	{
		DHCPDestroy(new_dhcp);
		new_dhcp = NULL;
		
		return NULL;
	}
	
	return 	new_dhcp;
}

/*------------------------------ DHCPDestroy ---------------------------------*/

void DHCPDestroy(dhcp_t *dhcp)
{
	assert(NULL != dhcp);
	
	DestroyNodes(dhcp->root);
	
	free(dhcp);
	dhcp = NULL;	
}

/*----------------------------- DHCPAllocateIP -------------------------------*/

status_t DHCPAllocateIP(dhcp_t *dhcp, const char *requested_ip, char *res_ip)
{
	status_t status = SUCCESS;
	unsigned int net_ip_bin = 0;
	unsigned int req_ip_bin = 0;
	char *res = NULL;
	struct in_addr total_ip = {0};

	assert(NULL != dhcp);
	assert(NULL != res_ip);

	inet_pton(AF_INET, dhcp->net_ip, &net_ip_bin);

	if (NULL == requested_ip)
	{
		req_ip_bin = net_ip_bin;
	}
	else if (1 != inet_pton(AF_INET, requested_ip, &req_ip_bin))
	{
		status = INVALID_ADDRESS;
		req_ip_bin = net_ip_bin;
	}
	
	net_ip_bin = htonl(net_ip_bin);
	net_ip_bin &= dhcp->subnet_mask;
	
	req_ip_bin = htonl(req_ip_bin);
	
	while ((net_ip_bin == (req_ip_bin & dhcp->subnet_mask)) &&
			(OCCUPIED_ADDRESS == CreateAddress(dhcp, &req_ip_bin)))
	{
		++req_ip_bin;
		status = (INVALID_ADDRESS != status) ? 
				 OCCUPIED_ADDRESS : INVALID_ADDRESS;
	}
	
	if (net_ip_bin != (req_ip_bin & dhcp->subnet_mask))
	{
		status = NO_AVAIL_IP;
	}
	total_ip.s_addr = ntohl(req_ip_bin);
	res = inet_ntoa(total_ip);
	strcpy(res_ip, res);
	
	return status;
}

/*------------------------- DHCPCountFreeAddresses ---------------------------*/

size_t DHCPCountFreeAddresses(const dhcp_t *dhcp)
{
	size_t allocated_mount = 0;
	size_t total_mount = 0;
	
	assert(NULL != dhcp);
	
	total_mount = ~(dhcp->subnet_mask) + ZERO_ADDRESS;
	
	allocated_mount = GetAllocatedSize(dhcp->root);
	
	return total_mount - allocated_mount;
}

/*------------------------------- DHCPFreeIP ---------------------------------*/

status_t DHCPFreeIP(dhcp_t *dhcp, const char *ip)
{
	status_t status = DOUBLE_FREE;
	unsigned int num_ip = 0;
	unsigned int mask = 1;
	unsigned int dir = 0;
	int i  = 0;
	node_trie_t *runner = NULL;
	
	assert(NULL != dhcp);
	assert(NULL != ip);
	
	if (1 != inet_pton(AF_INET, ip, &num_ip) ||
		!IsIPBelongsToNetwork(dhcp, ip))
	{
		return INVALID_ADDRESS;
	}
	
	num_ip = htonl(num_ip);
	runner = dhcp->root;
	mask <<= (dhcp->trie_depth - 1);
	
	for (i = dhcp->trie_depth - 1; 0 <= i; --i, mask >>= 1)
	{
		dir = (mask & num_ip) >> i;
		
		if (NULL == runner->child[dir])
		{
			return INVALID_ADDRESS;
		}
		
		runner->is_allocated = FALSE;
		runner = runner->child[dir];
	}
	
	if (runner->is_allocated)
	{
		runner->is_allocated = FALSE;
		status = SUCCESS;
	}
	
	return status;
}

/*----------------------------------------------------------------------------*/
/*                                HELPER FUNCTIONS                            */
/*----------------------------------------------------------------------------*/

/*------------------------------ CountSetBits --------------------------------*/
static size_t CountSetBits(unsigned int num)
{
	size_t counter = 0;

	while (0 != num)
	{
		num &= (num - 1);
		++counter;
	}

	return counter;
}

/*-------------------------------- NodeInit ----------------------------------*/

static void NodeInit(node_trie_t *node)
{
	assert(NULL != node);
	
	node->is_allocated = FALSE;
	node->child[ON] = NULL;
	node->child[OFF] = NULL;
}

/*-------------------------------- DHCPInit ----------------------------------*/

static int DHCPInit(dhcp_t *new_dhcp, const char *net_ip,
													   unsigned int subnet_mask)
{
	int status = SUCCESS;
	unsigned int net_addr = 0;
	unsigned int broad_addr = ~subnet_mask;
	unsigned int serv_addr = ~subnet_mask - 1;
	node_trie_t *new_node = NULL;

	assert(NULL != new_dhcp);
	assert(NULL != net_ip);

	new_node = (node_trie_t *)malloc(sizeof(node_trie_t));
	if (NULL == new_node)
	{
		return SYS_FAULT;
	}
	
	NodeInit(new_node);
	
	new_dhcp->root = new_node;
	new_dhcp->net_ip = (char *)net_ip;
	new_dhcp->subnet_mask = subnet_mask;
	new_dhcp->trie_depth = CountSetBits(~subnet_mask);

	if ((FAIL == CreateAddress(new_dhcp, &net_addr)) ||
		(FAIL == CreateAddress(new_dhcp, &broad_addr)) ||
		(FAIL == CreateAddress(new_dhcp, &serv_addr)))
	{
		status = SYS_FAULT;
	}
	
	return status;
}

/*-------------------------------- CreateNode --------------------------------*/
			 
static node_trie_t *CreateNode(void)
{
	node_trie_t *new_node = (node_trie_t *)malloc(sizeof(node_trie_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	
	NodeInit(new_node);
	
	return new_node;
}

/*------------------------------ CreateAddress --------------------------------*/

static int CreateAddress(dhcp_t *dhcp, unsigned int *ip)
{
	node_trie_t *runner = NULL;
	unsigned int mask = 1;
	int status = SUCCESS;
	int i  = 0;
	
	assert(NULL != dhcp);
	assert(NULL != ip);
	
	runner = dhcp->root;
	mask <<= (dhcp->trie_depth - 1);
	
	for (i = dhcp->trie_depth - 1; 0 <= i; --i, mask >>= 1)
	{
		unsigned int next_bit = (mask & *ip) >> i;
		
		if (NULL == runner->child[next_bit])
		{
			runner->child[next_bit] = CreateNode();
			if (NULL == runner->child[next_bit])
			{
				return SYS_FAULT;
			}
		}
		else if (NULL != runner->child[next_bit ^ 1] &&
				runner->child[OFF]->is_allocated &&
				runner->child[ON]->is_allocated)
		{
			runner->is_allocated = TRUE;
			return OCCUPIED_ADDRESS;
		}
		runner = runner->child[next_bit];
	}
	
	if (TRUE == runner->is_allocated)
	{
		status = OCCUPIED_ADDRESS;
	}
	runner->is_allocated = TRUE;
	
	return status;
}

/*------------------------------ DestroyNodes --------------------------------*/

static void DestroyNodes(node_trie_t *node)
{
	if (NULL == node)
	{
		return;
	}
	
	DestroyNodes(node->child[OFF]);
	DestroyNodes(node->child[ON]);
	
	free(node);
	node = NULL;
}

/*---------------------------- GetAllocatedSize ------------------------------*/

static size_t GetAllocatedSize(node_trie_t *node)
{
	if (NULL == node)
	{
		return 0;
	}
	
	if ((NULL == node->child[OFF]) && (NULL == node->child[ON]))
	{
		return node->is_allocated;
	}
	
	return (GetAllocatedSize(node->child[OFF]) + 
											 GetAllocatedSize(node->child[ON]));
}

/*--------------------------- IsBelongToNetwork ------------------------------*/

static int IsIPBelongsToNetwork(dhcp_t *dhcp, const char *requested_ip)
{
	unsigned int net_ip = 0;
	unsigned int req_ip = 0;
	
	assert(NULL != dhcp);
	
	if (1 == inet_pton(AF_INET, requested_ip, &req_ip) &&
		1 == inet_pton(AF_INET, dhcp->net_ip, &net_ip))
	{
		net_ip = htonl(net_ip);
		req_ip = htonl(req_ip);
		net_ip &= dhcp->subnet_mask;
		req_ip &= dhcp->subnet_mask;
	}
	
	return (net_ip == req_ip);	
}


