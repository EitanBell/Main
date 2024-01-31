/*************************************
*				     *
*	Developer: Eitan Bellaiche   *	 
*	Reviewer:  Noy Elankry       *
*	Date: 22/11/22	             *
*	Valgrind & GDB passed        *
*	Subject: Struct	             *
*				     *
**************************************/
/* you should add header guard because it's a multi file program*/
#include <stdio.h> /*for prinft*/
	

#define MAX2(a, b) (a > b ? a: b)
#define MAX3(a, b, c) (MAX2(a, b) && MAX2(a, c)? a: MAX2(b, c))
#define SIZEOF_VAR(obj) (char *)(&obj+1) - (char *)(&obj)
#define SIZEOF_TYPE(type) (size_t)(1 + (type*)0) 
#define ARR_LEN (3)
#define NULL_TER_CNT (1)

typedef int (*add_func_t)(void **data, int num);
typedef void (*print_func_t)(const void *data);
typedef void (*cleanup_func_t)(void **data);

typedef struct element
{
	void *data;
	add_func_t add_f_p;
	print_func_t print_f_p;
	cleanup_func_t cleanup_f_p;

}element_t;

enum operate_func{FAIL = -1, SUCCESS = 0};	

void InitArrayOfElements(element_t *array_of_elem);
int AddToInt(void **data, int num);
int AddToFlaot(void **data, int num);
int AddToString(void **data, int num);
void CleanUpIntFloat();
void CleanUpString(void **data);
void PrintInt(const void *data);
void PrintFloat(const void *data);
void PrintString(const void *data);
size_t GetLenInt(int num);
void StructTest(void);
void MacrosTest(void);

