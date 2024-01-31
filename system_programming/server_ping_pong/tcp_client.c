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

/*-------------------------------- TCPClient ---------------------------------*/

int main(int argc, char **argv)
{
    int sockfd = 0; 
    int port = 0;
    int i = 0;
    int msg_times = 0;
    char server_msg[MAX_LEN] = {0};
 	char *message = "Ping";
    struct sockaddr_in servaddr = {0};
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(FAIL == sockfd)
    {
        perror("Create socket failed");
        close(sockfd);
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

    if (0 > connect(sockfd, (struct sockaddr *)&servaddr, 
													sizeof(struct sockaddr_in)))
    {
        perror("Connect socket failed");
        close(sockfd);
        return FAIL;
    }

    msg_times = MIN_MSG + rand() % MAX_MSG;

    for (i = 0; msg_times > i; ++i)
    {
        if (0 > send(sockfd, message, MAX_LEN, 0))
        {
            perror("send message to socket failed");
            close(sockfd);
            return FAIL;
        }
        if (-1 == recv(sockfd, server_msg, MAX_LEN, 0))
        {
            perror("Recieve message from socket failed");
            close(sockfd);
            return FAIL;
        }

        printf("Server message: %s\n", server_msg);
        sleep(MIN_MSG + rand() % MAX_MSG);
    }

    close(sockfd);

    return 0;
}

