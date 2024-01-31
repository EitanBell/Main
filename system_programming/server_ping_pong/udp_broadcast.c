/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Aviv	                  *
*	Date: 09/05/23	                  *
*	Subject: Ping Pong Server  		  *
*				                	  *
**************************************/
#include <stdio.h>		/* printf, scanf */
#include <assert.h>     /* assert */
#include <unistd.h>     /* sleep */
#include <strings.h>	/* bzero */
#include <string.h>		/* strlen */

#include <sys/time.h>   /* struct timeval, rand */
#include <sys/types.h>	/* recvfrom */
#include <sys/socket.h> /* socket, connect, send, recv, close, setsockopt */
#include <netinet/in.h>	/* sockaddr_in */
#include <arpa/inet.h>	/* inet_addr, htons */

#define MAX_LEN (1024)
#define MIN_MSG (3)
#define MAX_MSG (7)

enum status
{
	FAIL = -1,
	SUCCESS
};

static void InitServaddrAddr(struct sockaddr_in *servaddr, int port);

/*------------------------------- UDPBroadcast -------------------------------*/

int main(void)
{
    int sockfd = 0; 
    int port = 0;
	int bytes_recieved = 0;
	int broadcast_enable = 1;
    char back_msg[MAX_LEN] = {0};
 	char *message = "Ping";
    struct sockaddr_in servaddr = {0};
	socklen_t serv_len = 0;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (FAIL == sockfd) 
    {
		perror("Create socket failed");
        return FAIL;
    }

    printf("Enter Port Number: ");
    scanf("%d", &port);

    if (FAIL == setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_enable,
													  sizeof(broadcast_enable))) 
    {
		perror("setsockopt failed");
		close(sockfd);
        return FAIL;
    }

	InitServaddrAddr(&servaddr, port);

	if (FAIL == sendto(sockfd, message, strlen(message), 0, 
        						 (struct sockaddr*)&servaddr, sizeof(servaddr))) 
    {
		perror("sendto failed");
		close(sockfd);
        return FAIL;
    }

	serv_len = sizeof(servaddr);
    bytes_recieved = recvfrom(sockfd, (char *)back_msg, MAX_LEN, MSG_WAITALL, 
			       		 (struct sockaddr *) &servaddr, (socklen_t *)&serv_len);
    if (FAIL == bytes_recieved)
    {
		perror("recvfrom failed");
		close(sockfd);
        return FAIL;
    }

    back_msg[bytes_recieved] = '\0';

	printf("Server: %s\n", back_msg);

    close(sockfd);

    return 0;
}

/*---------------------------- InitServaddrAddr ------------------------------*/

static void InitServaddrAddr(struct sockaddr_in *servaddr, int port)
{
    assert(NULL != servaddr);

	bzero(servaddr, sizeof(struct sockaddr_in));
    servaddr->sin_family = AF_INET;
    servaddr->sin_addr.s_addr = htonl(INADDR_BROADCAST);
    servaddr->sin_port = htons(port);
}
