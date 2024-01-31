/*******************************************
*				   						   *
*	Developer: Eitan Bellaiche      	   *	 
*	Reviewer:  		                   	   *
*	Date: 07/05/23	             	       *
*	Subject: TCP UDP ping pong			   *
*				                		   *
*******************************************/

#ifndef __TCP_UDP_H__
#define __TCP_UDP_H__

#include <netinet/in.h>		/* struct sockaddr_in */

int UDPServer(int port);

int UDPClient(const char *ip, int port);

int TCPServer(int port);

int TCPClient(const char *server_id, int port);

int UDPBroadcast(int port);

#endif /* __TCP_UDP_H__ */
