/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Gal	                  *
*	Date: 27/02/23	                  *
*	Subject: TCP UDP ping pong		  *
*				                	  *
**************************************/

#include "tcp_udp_ping_pong.h"

#define PORT_NUM (8080)

int main(void)
{
	TCPServer(PORT_NUM); 
	
	return 0;
}


