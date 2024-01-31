/****************************************
*				   			    	    *
*	Developer: Eitan Bellaiche          *	 
*	Reviewer:  Yuval                    *
*	Date: 08/01/23	                    *
*	Valgrind & gdb passed   	        *
*	Subject: variable Size Allocator    *
*				                  	    *
*****************************************/

#include <stdio.h>  /* printf */
#include <stdlib.h> /* malloc free */
#include <limits.h> /* CHAR_BIT */

#include "vsa.h"	/* header of this file */

#define IsAligned(adress) ((0 == (((size_t)adress) & (CHAR_BIT - 1))))

enum status
{
	SUCCESS = 0,
	FAIL = -1
};

static void VsaIsAlignedTest(void);
static void AllocateAndFreeTest(void);
static void AllocateWhenFull(void);
static void IntegrationTest(void); 

int main(void)
{
	VsaIsAlignedTest();
	AllocateAndFreeTest();
	AllocateWhenFull();
	IntegrationTest();
	return 0;
}

static void VsaIsAlignedTest(void)
{
	int pool[512] = {0};
	vsa_t *vsa = NULL;
	
	vsa = VSAInit(pool, 512);
	
	printf("FSA IsAligned Test:\n");
	
	printf("Pool align Test: ");
	
	(0 == ((char *)vsa - (char *)pool)) ? printf("Passed ✅️ ") :
	 					  				  printf("Failed ❌️ ");
	
	(IsAligned(vsa)) ? printf("Passed ✅️\n") :
	 				   printf("Failed ❌️\n");
	
	
	vsa = VSAInit((char *)pool + 2, 512);

	printf("Pool not align Test: ");

	(8 == ((char *)vsa - (char *)pool)) ? printf("Passed ✅️ ") :
	 					  				  printf("Failed ❌️ ");
	
	(IsAligned(vsa)) ? printf("Passed ✅️\n") :
	 				   printf("Failed ❌️\n");
	 					   					  
}



static void AllocateAndFreeTest(void)
{
	void *pool = NULL;
	vsa_t *vsa = NULL;
	void *address_arr[6] = {0};
	int status = 0;
	size_t i = 0;
	size_t pool_size = 512;
	size_t header_size = 16;
	size_t vsa_ms_size = 8;
	size_t free_space = 0;
	
	pool = malloc(pool_size);
	
	vsa = VSAInit(pool, pool_size);
	
	free_space = pool_size - header_size - vsa_ms_size;
	
	for (i = 0; i < 6; ++i)
	{

		address_arr[i] = VSAAllocate(vsa, ((i + 1) * 7));
		free_space = free_space - ((i + 1)*8 + 16);
		status += (free_space != VSAGetMaxFreeBlockSize(vsa));		
		status += (!IsAligned(address_arr[i]));	
	}
	
	
	
	for (i = 0; i < 5; ++i) /* white box - test */
	{
		status += ((((int)i + 2)*8 + 8) != ((char *)address_arr[i + 1] - (char *)address_arr[i]));
	}
	
	printf("Allocate Test: ");
	
	(SUCCESS == status) ? printf("Passed all tests ✅️\n") :
	 					  printf("Failed in %d tests❌️\n", status);
	
	
	printf("Free Test: ");
		
	for (i = 0; i < 6; ++i)
	{
		status += (free_space != VSAGetMaxFreeBlockSize(vsa));
		VSAFree(address_arr[i]);		
	}
	
	free_space = pool_size - header_size - vsa_ms_size;
	status += (free_space != VSAGetMaxFreeBlockSize(vsa));
	
	(SUCCESS == status) ? printf("Passed all tests ✅️\n") :
	 					  printf("Failed in %d tests❌️\n", status);
	 					  
	free(pool);	

}


static void AllocateWhenFull(void)
{
	void *pool = NULL;
	vsa_t *vsa = NULL;
	size_t pool_size = 512;
	void *test = NULL;
	size_t free_space = 0;

	printf("Allocate When Full test: ");
	
	pool = malloc(pool_size);
	
	vsa = VSAInit(pool, pool_size);
	
	free_space = VSAGetMaxFreeBlockSize(vsa);
	
	test = VSAAllocate(vsa, free_space);
	
	test = VSAAllocate(vsa, 5);
	

	
	(NULL == test) ? printf("Passed ✅️\n") :
	 				 printf("Failed ❌️\n");
	 					  
	free(pool);	
	
}


static void IntegrationTest(void)
{
	void *pool = NULL;
	vsa_t *vsa = NULL;
	void *address_arr[6] = {0};
	int status = 0;
	size_t pool_size = 549;

	printf("-----------------------\n");
	printf("---Integration Test----\n");
	printf("-----------------------\n");

	pool = malloc(pool_size + 2);
	vsa = VSAInit((char *)pool + 2, pool_size);
	
	status += (!IsAligned(vsa));
	
	status += (8 != (char *)vsa - (char *)pool);
	
	address_arr[0] = VSAAllocate(vsa, 25);
	address_arr[1] = VSAAllocate(vsa, 59); 
	address_arr[2] = VSAAllocate(vsa, 15);
	
	VSAGetMaxFreeBlockSize(vsa);
	 				   
	address_arr[3] = VSAAllocate(vsa, 68);
	address_arr[4] = VSAAllocate(vsa, 2); 
	
	VSAGetMaxFreeBlockSize(vsa);
	
	address_arr[5] = VSAAllocate(vsa, 500); 		   

	status += (NULL != address_arr[5]);
	
	VSAFree(address_arr[0]);
	
	address_arr[0] = VSAAllocate(vsa, 25);
	address_arr[1] = VSAAllocate(vsa, 59); 
	VSAFree(address_arr[1]);
	VSAFree(address_arr[2]);
	
	VSAGetMaxFreeBlockSize(vsa);
	
	VSAFree(address_arr[3]);
	address_arr[2] = VSAAllocate(vsa, 15); 				   
	address_arr[3] = VSAAllocate(vsa, 68);
	VSAFree(address_arr[4]);	
	
	(SUCCESS == status) ? printf("Passed all tests ✅️\n") :
	 					  printf("Failed in %d tests❌️\n", status);	


	free(pool);
}


