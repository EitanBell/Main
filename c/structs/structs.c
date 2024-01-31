/*************************************
*				     *
*	Developer: Eitan Bellaiche   *	 
*	Reviewer:  Noy Elankry       *
*	Date: 22/11/22	             *
*	Valgrind & GDB passed        *
*	Subject: Struct	             *
*				     *
**************************************/

#include <stdio.h> /*for prinft*/
#include <stdlib.h> /*for malloc*/
#include <string.h> /*for strcat*/
#include <assert.h> /*for assert*/

#include "structs.h" /*for my structs*/ 	

void StructTest(void)
{
	int i = 0;
	element_t array[ARR_LEN] = {0};
	
	InitArrayOfElements(array);
	
	printf("\n------- Test: Struct Elements --------\n");
	
	for (i = 0; i < 3; ++i)		
	{
		array[i].print_f_p(array[i].data);
	}
	for (i = 0; i < 3; ++i)			
	{
		array[i].add_f_p(&(array[i].data), 10);
		array[i].print_f_p(array[i].data);
		
	}
	for (i = 0; i < 3; ++i)		
	{
		array[i].print_f_p(array[i].data);
		array[i].cleanup_f_p(array[i].data);
		
	}
}

void InitArrayOfElements(element_t *array_of_elem)
{	
	char *str = "EitanBell";
		
	*((int *)(&(array_of_elem -> data))) = 5;
	array_of_elem -> add_f_p = &AddToInt;
	array_of_elem -> print_f_p = &PrintInt;
	array_of_elem -> cleanup_f_p = &CleanUpIntFloat;
	
	*((float *)&((array_of_elem + 1) -> data)) = 5.7;
	(array_of_elem + 1) -> add_f_p = &AddToFlaot;
	(array_of_elem + 1) -> print_f_p = &PrintFloat;
	(array_of_elem + 1) -> cleanup_f_p = &CleanUpIntFloat;
	
	(array_of_elem + 2) -> data = malloc((strlen(str) + 1) * sizeof(char));
	if (NULL == (array_of_elem + 2) -> data)
	{	
		return; 
	}
	
	strcpy(*((char **)(&((array_of_elem + 2) -> data))), str);
	(array_of_elem + 2) -> add_f_p = &AddToString;
	(array_of_elem + 2) -> print_f_p = &PrintString;
	(array_of_elem + 2) -> cleanup_f_p = &CleanUpString;
}


int AddToInt(void **data, int num)
{	
	assert(NULL != data);
	
	*((int *)(data)) += num;
	return SUCCESS;
}

int AddToFlaot(void **data, int num)
{
	assert(NULL != data);
	
	*((float *)(data)) += num;
	return SUCCESS;
}

int AddToString(void **data, int num)
{
	char *num_to_chars = NULL;
	size_t data_len = strlen(*((char **)(data))) + NULL_TER_CNT;
	size_t num_len = GetLenInt(num) + NULL_TER_CNT;
	
	assert(NULL != data);
	
	num_to_chars = (char *)malloc(sizeof(char) * num_len);
	if (NULL == num_to_chars)
	{
		return FAIL;
	}
	
	sprintf(num_to_chars, "%d", num);
	
	*((char **)(data)) = realloc(*((char **)(data)),
			     (data_len + num_len - 1) * sizeof(char));
	if (NULL == *((char **)(data)))
	{
		return FAIL;
	}
	
	strcat(*((char **)(data)), num_to_chars);
	
	free(num_to_chars);
	num_to_chars = NULL;
	
	return SUCCESS;
}

static size_t GetLenInt(int num)
{
	size_t counter = 0;
	
	if (0 == num)
	{
		return 0;
	}
	while (0 != num)
	{
		num /= 10;
		++counter;
	}
	return counter;
}
void CleanUpIntFloat(void)
{
	
}
void CleanUpString(void **data)
{
	assert(NULL != data);
	
	free(data);
	data = NULL;
}

void PrintInt(const void *data)
{
	assert(NULL != data);
	printf("%d\n", *((int *)(&data)));
}
void PrintFloat(const void *data)
{
	assert(NULL != data);
	printf("%f\n", *((float *)(&data)));
}
void PrintString(const void *data)
{
	assert(NULL != data);
	printf("%s\n", *(char **)(&data));
}

void MacrosTest(void)
{
	int x = 5, y = 7, z = 22;
	
	printf("\n------ Macros tests -------\n");
	
	printf("--- %lu ---\n", SIZEOF_VAR(x));
	printf("--- sizeof(char *) is: %lu ---\n", SIZEOF_TYPE(char *));
	printf("--- The Max between %d and %d is: %d ---\n", x, y, MAX2(x, y));
	printf("--- The Max between %d, %d, %d is: %d ---\n",
		 x, y, z, MAX3(x, y, z));
	
}



















