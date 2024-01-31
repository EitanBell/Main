/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Aviv	                  *
*	Date: 03/05/23	                  *
*	Subject: IPC Ping Pong			  *
*				                	  *
**************************************/

#include <stdio.h>		/* printf */
#include <unistd.h>     /* write */
#include <string.h>		/* strlen */
#include <fcntl.h>		/* open */
#include <stdlib.h>     /* exit */
#include <sys/types.h>	/* mkfifo */
#include <sys/stat.h>	/* mkfifo */

#define FIFO_FILE "/tmp/fifo"

static const char *g_buf1 = "Pong\n";
static char g_buf2[6];
static int g_fd = 0;

static void ReadFile();
static void WriteToFile();

/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/		
		 			 
int main(void)
{
	size_t i = 0;
	
	mkfifo(FIFO_FILE, 0660);
	
	for (i = 0; 5 > i; ++i)
    {
        while(sleep(1));
        ReadFile();
        printf("Ping Recived: %s\n", g_buf2);
        WriteToFile();
    }
    
	return 0;
}

/*----------------------------------------------------------------------------*/
/*                                HELPER FUNCTIONS                            */
/*----------------------------------------------------------------------------*/

static void ReadFile(void)
{
	if (0 >= (g_fd = open(FIFO_FILE, O_RDONLY)))
	{
		perror("File failed to open\n");
		exit(1);
	}

	if (0 >= read(g_fd, g_buf2, 6))
	{
		perror("Read from file failed\n");
		exit(1);
	}

	close(g_fd);
}

static void WriteToFile(void)
{
	if (0 >= (g_fd = open(FIFO_FILE, O_WRONLY)))
	{
		perror("File failed to open\n");
		exit(1);
	}

	if (0 >= write(g_fd, g_buf1, 6))
	{
		perror("Write to file failed\n");
		exit(1);
	}

	close(g_fd);    
}


