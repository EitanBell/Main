
/*********************************
*				    			 *
*	Developer: Eitan Bellaiche   *	 
*	Reviewer:  Gad               *
*	Date: 18/11/22	             *
*	     			     		 *
**********************************/


#include <stdio.h>		/* prints */
#include <string.h>		/* strlen */
#include <assert.h>		/* assert */
#include <stdlib.h>		/* malloc */
#include <ctype.h>		/* tolower*/

#define ROWS (4)
#define COLUMNS (2)	
#define ALIVE 1
#define DEAD 0
#define INTERVAL 7
#define SOLDIERS_SIZE 1500000
#define FIRST_KILLER (1)
#define EOF (-1)
#define SIZE (100)
#define NULL_TERM (1)

int GetArraySize(int arr[]);
int JosephusProblem(int soldiers[], int size);
int *SumArr2D(int mat[][COLUMNS], int rows, int columns, int rows_sum[]);
void TestSumArr2D();
void Swap(int*, int*);		
int *CopyArray(int *arr, int size);	
void SwapSizeT(size_t *, size_t *);		
void SwapTwo(size_t **a, size_t **b);
static int EnvpDupLowerCase(char **envp);
static size_t SizeOfEnvp(char **envp);
static int AllocateAndCopyStrings(char **buffer, char **envp);
static void ToLowerStrings(char **buffer);
static void PrintStrings(char **buffer);
static void FreeStrings(char **buffer);
static void TestJoseph(void);

int main(int argc, char *argv[], char *envp[])
{
	assert(argc);
	assert(argv);
	
/*	EnvpDupLowerCase(envp);*/
	TestJoseph();	
 	
	return 0;
}

int *SumArr2D(int mat[][COLUMNS], int rows, int columns, int rows_sum[])
{
	int i = 0, j = 0;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < columns; j++)
		{
			rows_sum[i] += mat[i][j];
		}
	}
	return rows_sum;
} 

void TestSumArr2D()
{
	int mat[ROWS][COLUMNS] = { {1, 2}, {3, 4}, {5, 6}, {7, 8} };
	int rows_sum[ROWS] = {0};
	int i = 0;
	int *ptr_to_rows_sum = NULL;
	
	ptr_to_rows_sum = SumArr2D(mat, ROWS, COLUMNS, rows_sum);
	
	for (i = 0; i < ROWS; ++i)
	{
		printf("%d  ", *(ptr_to_rows_sum + i)); 
	}
}


int JosephusProblem(int soldiers[], int soldiers_amount)
{
	int index = 0, last_alive = 0;
	int interval = INTERVAL;
	int num_alive = soldiers_amount;
	
	for (index = 0; index < soldiers_amount; ++index)
	{
		soldiers[index] = ALIVE;
	}
	index = FIRST_KILLER;
	
	while (num_alive > 1)
	{
		if (INTERVAL == interval)
		{	
			soldiers[index] = DEAD;
			--num_alive;
			interval = 0;
		}
		if ((index + 1) > soldiers_amount - 1)
		{
			index = -1;
		}
		++index;
		
		if (ALIVE == soldiers[index])
		{
			++interval;
			if (INTERVAL != interval)
			{
				last_alive = index;
			}
		}
	}
	return last_alive;
}

static void TestJoseph(void)
{
	int arr[7] = {1, 1, 1, 1, 1, 1, 1};
	
	printf("The last soldier survived is: %d\n", JosephusProblem(arr, 7));
}	



void SwapSizeT(size_t *a, size_t *b)
{	
	size_t temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void SwapTwo(size_t **a, size_t **b)
{	
	SwapSizeT(*a, *b);
}

void Swap(int *a, int *b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

int *CopyArray(int *arr, int size)
{
	int i = 0;
	
	int *copy_array = (int*)malloc(size * sizeof(int));
	if (NULL == copy_array)
	{
		return NULL;
	}
	
	for (i = 0; i < size; ++i)
	{
		*(copy_array + i) = *(arr + i);
	}
	
	return copy_array;
}


static int EnvpDupLowerCase(char **envp)
{
	char **buffer = NULL;
	size_t arr_length = 0;
	
	assert (NULL != envp);
	
	arr_length = SizeOfEnvp(envp);
		
	buffer = (char **)malloc(arr_length * sizeof(char*));
	if(NULL == buffer)
	{
		free(buffer);
		return EOF;
	}		

	if (EOF == AllocateAndCopyStrings(buffer, envp))
	{
		return EOF;
	}

	ToLowerStrings(buffer);
	
	PrintStrings(buffer);

	FreeStrings(buffer);

	free(buffer);
	
	return 0;
}	
static size_t SizeOfEnvp(char **envp)
{
	char **envp_runner = envp;
	size_t size = 0;

	while (NULL != *envp_runner)
	{
		++envp_runner;
	}
	
	size = (size_t)((envp_runner - envp) + NULL_TERM);
	
	return size;
}

static int AllocateAndCopyStrings(char **buffer, char **envp)
{
	char **envp_runner = envp, **buffer_runner = buffer;
	size_t str_len = 0;

	while (NULL != *envp_runner)
	{
		str_len = strlen((char*)*envp_runner) + NULL_TERM;
		*buffer_runner = (char*)malloc(str_len * sizeof(char*));
		if (NULL == *(buffer_runner))
		{
			free (*(buffer_runner));
			return -1;
		}
		
		strcpy(*(buffer_runner), *(envp_runner));
		++envp_runner;
		++buffer_runner;
	}
		
	return 0;
}

static void ToLowerStrings(char **buffer)
{
	char **buffer_runner = NULL;
	char *str_runner = NULL;
	size_t ch = 0, str_len = 0;
	
	buffer_runner = buffer;
	str_runner = *buffer;
	
	while (NULL != *buffer_runner)
	{
		str_runner = *buffer_runner;
		str_len = strlen(str_runner);
		
		for (ch = 0; ch < str_len; ++ch)
		{
			*(str_runner + ch) = (char)tolower((int)*(str_runner + ch));
		}
		
		++buffer_runner;
	}
}

static void PrintStrings(char **buffer)
{
	char **buffer_runner = NULL;
	
	buffer_runner = buffer;
	while (NULL != *buffer_runner)
	{
		printf("%s\n", *buffer_runner);
		++buffer_runner;
	}
}

static void FreeStrings(char **buffer)
{
	char **buffer_runner = NULL;
	
	buffer_runner = buffer;
	
	while (NULL != *buffer_runner)
	{
		free(*buffer_runner);
		++buffer_runner;
	}
}

