/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Date: 01/03/23	                  *
*	Subject: Data Structure Algorithm *
**************************************/

#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */
#include <string.h> /* strcmp, strlen */
#include <stdlib.h> /* malloc, memcpy */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")
#define BOOL_TO_STATUS(res) (res ^ 1)

static int Test(void);
static char *StrNCircural(char *str, size_t n);
static int IsFound(char *str, char find_me_please);

int main (void)
{
	printf("\n\t******************** TESTS BEGIN ********************\n\n");
	printf("\t\tIsFound Test: \t\t%s\n", TEST_RES(Test()));
	printf("\t\tStrNCircural Test: \t%s\n", TEST_RES(Test()));
	printf("\n\t******************* TESTS FINISHED *******************\n\n");
	
	return 0;
}

static int Test(void)
{
	int status = 0;
	char str[10] = {'E', 'i', 't', 'a', 'n', 'B', 'e', 'l', 'l'};
	char res_2_shifts[10] = {'l', 'l', 'E', 'i', 't', 'a', 'n', 'B', 'e'};
	
	status = BOOL_TO_STATUS(IsFound("EitanBell", 'B'));
	
	if(NULL == StrNCircural(str, 2))
	{
		status = 1;	
	}
	
	status += strcmp(str, res_2_shifts);

	return status;
}

static int IsFound(char *str, char find_me_please)
{
	size_t i = 0;
	int status = 1;
	
	assert(NULL != str);
	
	for (i = 0; strlen(str) > i; ++i)
	{
		status *= (str[i] ^ find_me_please);
	}

	return (!status);
}

static char *StrNCircural(char *str, size_t n)
{
	size_t i = 0;
	size_t new_index = 0;
	size_t len = 0;
	char *tmp_str = NULL;
	
	assert(NULL != str);
	
	len = strlen(str);
	
	tmp_str = (char *)malloc(len * sizeof(char));
	if(NULL == tmp_str)
	{
		return NULL;	
	}
	
	for (i = 0; len > i; ++i)
	{
		new_index = (i + n) % len;
		tmp_str[new_index] = str[i];
	}
	memcpy(str, tmp_str, len);
	
	free(tmp_str);
	tmp_str = NULL;
	
	return str;
}

