/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Date: 01/03/23	                  *
*	Subject: Data Structure Algorithm *
**************************************/

#include <cstring>		// strcmp, memcpy
#include <iostream>		// std
#include <cassert> 		// assert

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")
#define BOOL_TO_STATUS(res) (res ^ 1)

using namespace std;

static int Test(void);
static void *Memcpy(void *dest, void *src, size_t size);

int main (void)
{
	std::cout << "\n\t******************** TESTS BEGIN *******************\n\n";
	
	std::cout << "\t\t      Memcpy Test:   " << TEST_RES(Test()) << std::endl;
							
	std::cout << "\n\t****************** TESTS FINISHED ******************\n\n";
	
	return 0;
}

static int Test(void)
{
	char name1[11] = "Test1 Bell";
	char name2[6] = "Eitan";
	char res[11] = "Eitan Bell";
	
	int status = 0;
	
	Memcpy(name1, name2, 5);
	
	status = (0 == strcmp(name1, res)) ? 0: 1;
	
	return status;
}

static void *Memcpy(void *dest, void *src, size_t size)
{
	char *p_dest = NULL;
	char *p_src = NULL;
	size_t i = 0;
	
	assert(NULL != dest);
	assert(NULL != src);
	
	p_dest = (char *)dest;
	p_src = (char *)src;
	
	for (i = 0; size > i; ++i)
	{
		*p_dest = *p_src;
		++p_dest;
		++p_src;
	}
	
	return dest;
}

