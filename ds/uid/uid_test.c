/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:  Matan                *
*	Date: 01/01/23	                 *
*	Valgrind & gdb passed   	       *
*	Subject: UID	  			  *
*				                 *
**************************************/

#include <stdio.h>	/* printf */

#include "uid.h"		/* all in use */

#define TEST_RES(status) (status ? "\xe2\x9c\x97\033[0;31m  FAIL\033[0m" : "\xE2\x9C\x93 \033[0;32m SUCCESS\033[0m")

static void RunTests(void);
static int UIDTest(void);

int main(void)
{
	RunTests();
	
	return 0;
}

static void RunTests(void)
{
	printf("\t\n****************** TESTS BEGIN ******************\n\n");
	
	printf("\t\tTest Results: %s\n", TEST_RES(UIDTest()));
	
	printf("\n\t****************** TESTS FINISHED ******************\n\n");
}

static int UIDTest(void)
{
	int status = 1;
	ilrd_uid_t id1 = UIDGenerate();
	ilrd_uid_t id2 = UIDGenerate();
	ilrd_uid_t id3 = UIDGenerate();
	
	if (UIDIsEqual(id1, id1) && !UIDIsEqual(id1, id2) &&
	    UIDIsEqual(id2, id2) && !UIDIsEqual(id1, id3) &&
	    UIDIsEqual(id3, id3) && !UIDIsEqual(id2, id3))
	{
		status = 0;
	}

	if (!UIDIsEqual(bad_uid, UIDGenerate()) && !status)
	{
		status = 0;
	}
	else
	{
		status = 1;
	}
	
	return status;
}

