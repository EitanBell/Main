/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Gal	                  *
*	Date: 27/02/23	                  *
*	Subject: TCP UDP ping pong		  *
*				                	  *
**************************************/
#include <stdio.h>		/* stdin, fgets */
#include <string.h>		/* strlen */
#include <stdlib.h>		/* fprintf */
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include <sys/types.h>	/* recvfrom */
#include <sys/socket.h> /* socket, connect, send, recv, listen, setsockopt */
#include <netinet/in.h>	/* sockaddr_in */
#include <arpa/inet.h>	/* inet_addr, htons */
#include <sys/wait.h>
#include <netdb.h>
#include <sys/time.h> /* timeval */

#include "tcp_udp_ping_pong.h"

#define _GNU_SOURCE
#define PORT_NUM (8080)
#define MAX_LEN (128)

enum status
{
	FAIL = -1,
	SUCCESS
};

static int CreateSocket(int type);
static int InitSocket(struct sockaddr_in *addr, const char *server_ip,
	 												 int port, int socket_type);
static int ConnectSocket(int sockfd, struct sockaddr_in *server_addr);
static int SendSocket(int sockfd, const char *msg);
static int ReceiveSocket(int sockfd, char* msg);

/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

/*-------------------------------- UDPServer ---------------------------------*/

int UDPServer(int port)
{
	char buff[MAX_LEN] = {0};
	int sockfd = 0;
	int addr_len = 0;
	int msg_len = 0;
	char *ping = "Ping";
	struct sockaddr_in server_addr = {0};
	struct sockaddr_in client_addr = {0};
	
	sockfd = InitSocket(&server_addr, NULL, port, SOCK_DGRAM);
	if (0 > sockfd) 
	{
		return FAIL;
	}
	
	if (0 > bind(sockfd, (const struct sockaddr *)&server_addr, 
														   sizeof(server_addr)))
	{
		close(sockfd);
		printf("hereeee\n");
        return FAIL;
	}
	
	addr_len = sizeof(client_addr); 
	
	while (1)
	{
		msg_len = recvfrom(sockfd, (char *)buff, MAX_LEN, MSG_WAITALL, 
					   (struct sockaddr *)&client_addr, (socklen_t *)&addr_len);
		buff[msg_len] = '\0';
		sleep(1);

		printf("Client : %s\n", buff);
		sendto(sockfd, (const char *)ping, strlen(ping), MSG_CONFIRM, 
							   (const struct sockaddr *)&client_addr, addr_len);
	}

	return SUCCESS;
}

/*-------------------------------- UDPClient ---------------------------------*/

int UDPClient(const char *server_addr, int port) 
{
	int sockfd = 0;
	int addr_len = 0;
	int msg_len = 0;
	int i = 0;
	char buff[MAX_LEN] = {0};
	char message[MAX_LEN] = {0};
	struct sockaddr_in servaddr = {0};
	
	sockfd = InitSocket(&servaddr, server_addr, port, SOCK_DGRAM);
	if (0 > sockfd)
	{
		return FAIL;
	}
	
	for (i = 0; 3 > i; ++i)
	{
		printf("Enter Your Message: ");
        fgets(message, MAX_LEN, stdin);
        
        sendto(sockfd, (const char *)message, strlen(message), MSG_CONFIRM, 
						 (const struct sockaddr *) &servaddr, sizeof(servaddr));
			
		msg_len = recvfrom(sockfd, (char *)buff, MAX_LEN, MSG_WAITALL, 
						  (struct sockaddr *)&servaddr, (socklen_t *)&addr_len);

		buff[msg_len] = '\0';
		sleep(1);

		printf("Server: %s\n", buff);
	}
	
	close(sockfd);
	
    return SUCCESS;
}

/*------------------------------ UDPBroadcast --------------------------------*/

int UDPBroadcast(int port)
{
    struct sockaddr_in broadcast_addr = {0};
	int sockfd = 0;
    int bd_enable = 1;
    char message[MAX_LEN]= {0};
	
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (FAIL == sockfd) 
    {
        return FAIL;
    }
    
    if (FAIL == setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, 
        						   		  &bd_enable, sizeof(bd_enable))) 
    {
        return FAIL;
    }
    
    broadcast_addr.sin_family = AF_INET;
	broadcast_addr.sin_port = htons(port);
    broadcast_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	
	while (strcmp(message, "\n")) 
    {
        printf("Enter message to broadcast: ");
        fgets(message, MAX_LEN, stdin);

        if (FAIL == sendto(sockfd, message, strlen(message), 0, 
           			 (struct sockaddr*)&broadcast_addr, sizeof(broadcast_addr))) 
        {
            return FAIL;
        }
    }

    close(sockfd);

    return 0;
}

/*-------------------------------- TCPServer ---------------------------------*/

int TCPServer(int port)
{
	int sockfd = 0;
    int accepted_socketfd = 0;
    int client_len = 0;
    struct sockaddr_in addr = {0};
    char client_message[MAX_LEN]= {0};
    const char *server_msg = "PING from Server\n";

	sockfd = InitSocket(&addr, NULL, port, SOCK_STREAM);
    if (FAIL == sockfd)
    {
        printf("Create socket falied");
        return FAIL;
    }
    
    if (0 > bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)))
    {
        return FAIL;
    }
    
    client_len = sizeof(struct sockaddr_in);
	
	listen(sockfd, 3);
	
	while (1)
    {
        printf("Listening for connections.\n");
        accepted_socketfd = accept(sockfd, (struct sockaddr *)&addr, 
                                    				   (socklen_t*)&client_len);
        if (0 > accepted_socketfd)
        {
            return FAIL;
        }
        printf("Connection accepted\n");

        memset(client_message, '\0', sizeof(client_message));
        
        if (0 > recv(accepted_socketfd, client_message, MAX_LEN, 0))
        {
            printf("recv failed");
            return FAIL;
        }

        printf("Client : %s\n", client_message);
        sleep(1);

        if (0 > send(accepted_socketfd, server_msg, strlen(server_msg), 0))
        {
            printf("send failed");
            return FAIL;
        }
        
        close(accepted_socketfd);
        return SUCCESS;
    }
}

/*-------------------------------- TCPClient ---------------------------------*/

int TCPClient(const char *server_id, int port)
{
	int sockfd = 0; 
 	char message[MAX_LEN] = {0};
    struct sockaddr_in server_addr = {0};
    char server_reply[MAX_LEN] = {0};

	sockfd = InitSocket(&server_addr, server_id, port, SOCK_STREAM);
    if(FAIL == sockfd)
    {
        printf("Create socket falied\n");
        return FAIL;
    }
    
    if (0 > ConnectSocket(sockfd, &server_addr))
    {
        perror("connect failed\n");
        return FAIL;
    }

	printf("Enter Your Message: ");
    fgets(message, MAX_LEN, stdin);
	
	SendSocket(sockfd, message);
    ReceiveSocket(sockfd, server_reply);
    printf("Server: %s\n",server_reply);
    sleep(1);

	close(sockfd);

    shutdown(sockfd,0);
    shutdown(sockfd,1);
    shutdown(sockfd,2);

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/*                                HELPER FUNCTIONS                            */
/*----------------------------------------------------------------------------*/

/*------------------------------ CreateSocket --------------------------------*/

static int CreateSocket(int type)
{
    int sockfd = 0;
    sockfd = socket(AF_INET, type, 0);

    return sockfd;
}

/*------------------------------- InitSocket ---------------------------------*/

static int InitSocket(struct sockaddr_in *addr, const char *server_ip,
	 										  		  int port, int socket_type)
{
    int sockfd = 0;
    in_addr_t af = 0;

    sockfd = CreateSocket(socket_type);

	if (NULL != server_ip)
    {
        af = inet_addr(server_ip); 
    }
    else
    {
        af = htonl(INADDR_ANY);
    }

	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	addr->sin_addr.s_addr = af;

    return sockfd;
}

/*----------------------------- ConnectSocket --------------------------------*/

static int ConnectSocket(int sockfd, struct sockaddr_in *server_addr)
{
    int status = SUCCESS;
    
    status = connect(sockfd, (struct sockaddr *)server_addr, 
                               					    sizeof(struct sockaddr_in));

    return status;
}

/*-------------------------------- SendSocket --------------------------------*/

static int SendSocket(int sockfd, const char *msg)
{
    int status = SUCCESS;
    struct timeval tv = {0};
    tv.tv_sec = 20;  
    tv.tv_usec = 0;
   
    if (0 > setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO,(char *)&tv, 
        sizeof(tv)))
    {
        printf("Time Out\n");
        return FAIL;
    }

    status = send(sockfd, msg, MAX_LEN, 0);
    
    return status;
}

/*----------------------------- ReceiveSocket --------------------------------*/

static int ReceiveSocket(int sockfd, char* msg)
{
    int status = SUCCESS;
    struct timeval tv = {0};
    tv.tv_sec = 20;  
    tv.tv_usec = 0;
    
    if (0 > setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, 
        sizeof(tv)))
    {
        printf("Time Out\n");
        return FAIL;
    }

    status = recv(sockfd, msg, MAX_LEN, 0);
    
    return status;
}


