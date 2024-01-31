/*************************************
*				     *
*	Developer: Eitan Bellaiche   *	 
*	Reviewer:  Tal               *
*	Date: 18/11/22	             *
*	     			     *
*				     *
**************************************/


#include <stdio.h>/*for printf tests*/
#include <ctype.h>/*for tolower*/
#include <stdlib.h>/*for malloc*/
#include <assert.h>/*for asserts*/

#define NULL_TERM (1)

size_t StrLen(const char *s);
int StrCmp(const char *s1, const char *s2);
char* StrCpy(char* dest, const char* source);
char *StrnCpy(char *dest, const char *src, size_t n);
int StrnCmp(const char *s1, const char *s2, size_t n);
int StrCaseCmp(const char *s1, const char *s2);
char *StrChr(const char *s, int c);
char *StrDup(const char *s);
char *StrCat(char *dest, const char *src);
char *StrnCat(char *dest, const char *src, size_t n);
char *StrStr(const char *haystack, const char *needle);
size_t StrSpn(const char *s, const char *accept);
int IsPalindrome(const char *s);
void AllFuncTests();
void StrLenTest();
void StrCmpTest();

int main(void)
{
	AllFuncTests();
	
	return 0;
}

size_t StrLen(const char *s)
{	
	size_t counter = 0;
	
	assert(NULL != s);
	
	while ('\0' != *s)
	{
		++counter;
		++s;
	}
	return counter;
}


int StrCmp(const char *s1, const char *s2)
{
	assert(NULL != s1);
	assert(NULL != s2);

	while ((*s1 == *s2) && (*s1 != '\0'))
	{
		++s1;
		++s2;
	}
	
	return *s1 - *s2;
}

char* StrCpy(char* dest, const char* source)
{
	char *ptr_d = NULL;
	char *ptr_s = NULL;
	
	assert(NULL != dest);
	assert(NULL != source);

	ptr_d = dest;
	ptr_s = (char *)source;
	
	while ('\0' != *ptr_s)
	{
		*ptr_d = *ptr_s;
		++ptr_d;
		++ptr_s;
	}
	return dest;
}
char *StrnCpy(char *dest, const char *src, size_t n)
{	
	char *ptr_d = NULL, *ptr_s = NULL;
	int i = 0, n_counter = 0;
	
	assert(NULL != dest);
	assert(NULL != src);

	ptr_d = dest;
	ptr_s = (char *)src;
	n_counter = (int)n;
	
	for (i = 0; i < (int)n; i++)		
	{
		if ('\0' == *ptr_s)	
		{
			ptr_d = NULL;
			++ptr_d;
			continue;
		}
		
		*ptr_d = *ptr_s;
		++ptr_d;
		++ptr_s;
		++n_counter;
	}
	return dest;			
}


int StrnCmp(const char *s1, const char *s2, size_t n)
{
	char *ptr_s1 = NULL, *ptr_s2 = NULL;
	int counter = 0;
	
	assert(NULL != s1);
	assert(NULL != s2);
	
	ptr_s1 = (char *)s1;
	ptr_s2 = (char *)s2;
	counter = n;
	
	while ('\0' != *ptr_s1 && '\0' != *ptr_s2 && counter)
	{
		++ptr_s1;
		++ptr_s2;
		--counter;
	}
	return *ptr_s1 - *ptr_s2;
}

int StrCaseCmp(const char *s1, const char *s2)
{
	char *ptr_s1 = NULL, *ptr_s2 = NULL;

	assert(NULL != s1);
	assert(NULL != s2);
	
	ptr_s1 = (char *)s1;
	ptr_s2 = (char *)s2;
	
	while ('\0' != *ptr_s1 && '\0' != *ptr_s2)
	{
		if (islower(*ptr_s1) != islower(*ptr_s2))
		{
			return *ptr_s1 - *ptr_s2;
		}
		++ptr_s1;
		++ptr_s2;
	}
	return 0;
}

char *StrChr(const char *s, int c)
{
	char *ptr_s = NULL;
	
	assert(NULL != s);
	
	ptr_s = (char *)s;
	
	while ('\0' != *ptr_s)
	{
		if (c == (int)*ptr_s)
		{
			return ptr_s;
		}
	}
	return NULL;
}

char *StrDup(const char *s)
{
	int size_s = 0;
	char *ptr_new = NULL;
	
	assert(NULL != s);
	
	size_s = StrLen(s);
	
	ptr_new = (char *)malloc(size_s * sizeof(char));
	if (NULL == ptr_new)
	{
		return NULL;
	}

	return StrCpy(ptr_new, s);
}


char *StrCat(char *dest, const char *src)
{
	char *ptr_d = NULL;
	char *ptr_s = NULL;
	int size_d = 0;
	
	assert(NULL != dest);
	assert(NULL != src);
	
	size_d = StrLen(dest);
	ptr_s = (char *)src;
	ptr_d = (char *)dest;
	ptr_d += size_d;
	
	while ('\0' != *ptr_s)
	{
		*ptr_d = *ptr_s;
		++ptr_d;
		++ptr_s;
	}
	return dest;
}

char *StrnCat(char *dest, const char *src, size_t n)
{
	char *ptr_d = NULL, *ptr_s = NULL;
	int size_d = 0, n_counter = 0;
	
	assert(NULL != dest);
	assert(NULL != src);
	
	size_d = StrLen(dest);
	ptr_s = (char *)src;
	ptr_d = dest;
	ptr_d += size_d;
	n_counter = (int)n;
	
	while ('\0' != *ptr_s && 0 != n_counter)
	{
		*ptr_d = *ptr_s;
		++ptr_d;
		++ptr_s;
		--n_counter;
	}
	*ptr_d = '\0';
	
	return dest;
}

char *StrStr(const char *haystack, const char *needle)
{	
	int needle_size = 0, haystack_size = 0;

	assert(NULL != haystack);
	assert(NULL != needle);	
	
	needle_size = StrLen(needle);
	haystack_size = StrLen(haystack);
	
	while ('\0' != *haystack && haystack_size >= needle_size)
	{
		if ((*haystack == *needle) && 
		    (0 == StrnCmp(haystack, needle, needle_size)))
		{
			return (char *)haystack;
		}
		++haystack;
		--haystack_size;
	}
	return NULL;	
}


size_t StrSpn(const char *s, const char *accept)
{
	int counter = 0;

	assert(NULL != s);
	assert(NULL != accept);
	
	while ('\0' != *s && NULL != StrChr(accept, *s))
	{
		++counter;
	}
	return counter;
}	

int IsPalindrome(const char *s)
{
	char *first = NULL, *last = NULL;

	assert(NULL != s);	
	
	first = (char *)s;
	last = (char *)s + (int)StrLen(s) - NULL_TERM;
	
	while (first < last)
	{
		if (*first != *last)
		{
			return -1;
		}
		++first;
		--last;
	}
	return 0;
}


void AllFuncTests()
{
	StrLenTest();
	StrCmpTest();
}

void StrLenTest()
{
	char *str1 = NULL;
	char *str2 = NULL;
	char *str3 = NULL;
	
	str1 = "eitanBell123";
	str2 = "";
	str3 = "1";
	
	printf("\n----- StrLen TESTS ------\n");
	printf("should be 12, is: %ld\n", StrLen(str1));
	printf("should be 0, is: %ld\n", StrLen(str2));
	printf("should be 1, is: %ld\n\n", StrLen(str3));
}

void StrCmpTest()
{
	char *str1 = "eitanBell123";
	char *str2 = "eitanbell123";
	
	printf("\n----- StrCmp TESTS ------\n");

	printf("Should be %d, is: %d\n",'B'-'b', StrCmp(str1, str2));
}
 	

















