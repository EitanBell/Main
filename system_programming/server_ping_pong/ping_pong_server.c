/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Aviv	                  *
*	Date: 09/05/23	                  *
*	Subject: Ping Pong Server  		  *
*				                	  *
**************************************/
#include <stdio.h>		/* stdin, fgets */
#include <assert.h>     /* assert */
#include <string.h>		/* strlen, strcmp */
#include <stdlib.h>		/* exit */
#include <unistd.h>     /* sleep */
#include <strings.h>	/* bzero */

#include <sys/time.h>   /* struct timeval */
#include <sys/types.h>	/* recvfrom */
#include <sys/socket.h> /* socket, connect, send, recv, listen */
#include <netinet/in.h>	/* sockaddr_in */
#include <arpa/inet.h>	/* inet_addr, htons */

#define MAX_LEN (1024)

enum status
{
	FAIL = -1,
	SUCCESS
};

static int PingPongServer(int port);
static int Max(int x, int y);
static void InitServerAddr(struct sockaddr_in *servaddr, int port);
static void SetFDToSocket(fd_set *socket_set, int listenfd, int udpfd);
static void TCPCommunicate(int socketfd, char *msg, char *client_msg);
static int UDPCommunication(int udpfd, char *msg, char *client_msg, 
                            struct sockaddr_in *cliaddr, socklen_t cliaddr_len);
static void PongBackHandler(fd_set *socket_set, char *client_msg);

/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------- main -----------------------------------*/
int main(void)
{
    int port = 0;

    printf("Enter Port Number: ");
    scanf("%d", &port);

    PingPongServer(port);

    return 0;
}

/*------------------------------ PingPongServer ------------------------------*/

static int PingPongServer(int port)
{
    int listenfd = 0;
    int connectfd = 0;
    int udpfd = 0;
    int maxfd = 0;
    char client_msg[MAX_LEN] = {0};
    char *pong = "pong";
    fd_set socket_set = {0};
    socklen_t cliaddr_len = 0;
    pid_t pid = 0;
    struct sockaddr_in servaddr = {0};
    struct sockaddr_in cliaddr = {0};

    if (0 > (listenfd = socket(AF_INET, SOCK_STREAM, 0)))
	{
		return FAIL;
	}

    InitServerAddr(&servaddr, port);

    if (0 > bind(listenfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)))
	{
		close(listenfd);
        return FAIL;
	}

    listen(listenfd, 3);

    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > udpfd) 
	{
		return FAIL;
	}

    if (0 > bind(udpfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)))
	{
		close(udpfd);
        return FAIL;
	}

    FD_ZERO(&socket_set);

    maxfd = Max(listenfd, udpfd) + 1;
    
    while (strcmp(client_msg, "quit\n"))
    {
         struct timeval timeval = {0};

        timeval.tv_sec = 7;
        timeval.tv_usec = 0;

        SetFDToSocket(&socket_set, listenfd, udpfd);

        cliaddr_len = sizeof(cliaddr);
        memset(client_msg, '\0', sizeof(client_msg));

        if (0 == select(maxfd, &socket_set, NULL, NULL, &timeval))
        {
            printf("No New Client Within 7 Seconds\n");
        }

        if (FD_ISSET(listenfd, &socket_set))
        {
            if (0 > (connectfd = accept(listenfd, (struct sockaddr *)&cliaddr,
                                                                 &cliaddr_len)))
            {
                return FAIL;
            }

            if (0 == (pid = fork()))
            {
                memset(client_msg, '\0', sizeof(client_msg));
                
                TCPCommunicate(connectfd, pong, client_msg);

                close(connectfd);
                printf("Client Disconnected\n");

                exit(0);
            }

            close(connectfd);
        }

        if (FD_ISSET(udpfd, &socket_set))
        {
            if (FAIL == UDPCommunication(udpfd, pong, client_msg, &cliaddr,
                                                                   cliaddr_len))
            {
                return FAIL;
            }
        }

        PongBackHandler(&socket_set, client_msg);
    }

        close(listenfd);
        close(connectfd);
        close(udpfd);

        return 0;
}


/*----------------------------------------------------------------------------*/
/*                              HELPER FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

/*----------------------------- InitServerAddr -------------------------------*/

static void InitServerAddr(struct sockaddr_in *servaddr, int port)
{
    assert(NULL != servaddr);

    bzero(servaddr, sizeof(struct sockaddr_in));
    servaddr->sin_family = AF_INET;
    servaddr->sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr->sin_port = htons(port);
}

/*---------------------------------- Max -------------------------------------*/

static int Max(int x, int y)
{
    return ((x > y) ? x : y);
}

/*----------------------------- SetFDToSocket --------------------------------*/

static void SetFDToSocket(fd_set *socket_set, int listenfd, int udpfd)
{
    assert(NULL != socket_set);

    FD_SET(listenfd, socket_set);
    FD_SET(udpfd, socket_set);
    FD_SET(STDIN_FILENO, socket_set);
}

/*---------------------------- TCPCommunicate --------------------------------*/

static void TCPCommunicate(int socketfd, char *msg, char *client_msg)
{
    assert(NULL != msg);
    assert(NULL != client_msg);

    while (0 < recv(socketfd, client_msg, MAX_LEN, 0))
    {
        printf("Client message: %s\n", client_msg);
        if (0 > send(socketfd, msg, strlen(msg), 0))
        {
            printf("Send message failed");
            exit(FAIL);
        }
    }
}

/*--------------------------- UDPCommunication -------------------------------*/

static int UDPCommunication(int udpfd, char *msg, char *client_msg, 
                             struct sockaddr_in *cliaddr, socklen_t cliaddr_len)
{
    int rcv_bytes = 0;

    assert(NULL != msg);
    assert(NULL != client_msg);
    assert(NULL != cliaddr);
    
    cliaddr_len = sizeof(cliaddr);
    rcv_bytes = recvfrom(udpfd, (char *)client_msg, MAX_LEN, 0, 
						              (struct sockaddr *)cliaddr, &cliaddr_len);
    if (FAIL == rcv_bytes)
    {
        return FAIL;
    }
    client_msg[rcv_bytes] = '\0';

    printf("Client message: %s\n", client_msg);

    if (FAIL == sendto(udpfd, (const char *)msg, strlen(msg), 0, 
                        (const struct sockaddr *)cliaddr, cliaddr_len))
    {
        printf("Send message failed");
        return FAIL;
    }

    return 0;
}

/*--------------------------- PongBackHandler --------------------------------*/

static void PongBackHandler(fd_set *socket_set, char *client_msg)
{
    assert(NULL != socket_set);
    assert(NULL != client_msg);

    if (FD_ISSET(STDIN_FILENO, socket_set))
    {
        fgets(client_msg, MAX_LEN, stdin);

        if (0 == strcmp("ping\n", client_msg))
        {
            printf("pong\n");
        }
    }
}



