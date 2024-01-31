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
#include <string.h>		/* strlen */
#include <stdlib.h>		/* rand */

#include <sys/time.h>   /* struct timeval, rand */
#include <sys/types.h>	/* recvfrom */
#include <sys/socket.h> /* socket, connect, send, recv, close */
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

/*-------------------------------- UDPClient ---------------------------------*/

int main(int argc, char **argv)
{
    int sockfd = 0; 
    int port = 0;
    int i = 0;
	int bytes_recieved = 0;
    int msg_times = 0;
    char server_msg[MAX_LEN] = {0};
 	char *message = "Ping";
    struct sockaddr_in servaddr = {0};
	socklen_t serv_len = 0; 
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(FAIL == sockfd)
    {
        perror("Create socket failed");
        return FAIL;
    }

    printf("Enter Port Number: ");
    scanf("%d", &port);

    if (NULL != argv[argc - 1])
    {
        servaddr.sin_addr.s_addr = inet_addr(argv[argc - 1]); 
    }
    else
    {
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);

    msg_times = MIN_MSG + rand() % MAX_MSG;
	serv_len = sizeof(servaddr);

    for (i = 0; msg_times > i; ++i)
    {
		sleep(MIN_MSG + rand() % MAX_MSG);

        if (FAIL == sendto(sockfd, (const char *)message, strlen(message),
            MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)))
        {
            perror("sendto socket failed");
            return FAIL;
        }

		bytes_recieved = recvfrom(sockfd, (char *)server_msg, MAX_LEN, 
			MSG_WAITALL, (struct sockaddr *) &servaddr, (socklen_t *)&serv_len);
		if (FAIL == bytes_recieved)
        {
			perror("recvfrom socket failed");
            return FAIL;
        }

		server_msg[bytes_recieved] = '\0';

        printf("Server message: %s\n", server_msg);
    }

    close(sockfd);

    return 0;
}




