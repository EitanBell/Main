


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int IsStringRotation(char *str1, char *str2);

int main()
{
	printf("--- %d ---\n",IsStringRotation("121235", "123412"));
	return 0;
}

int IsStringRotation(char *str1, char *str2)
{
	int size_str1 = 0;
	int size_str2 = 0;
	char *p_str1 = NULL;
	char *p_str2 = NULL;
	char *sub_check = NULL;
	int sub_counter = 0, tries_count = 0;
	int i = 0;
	
	size_str1 = strlen(str1);
	size_str2 = strlen(str2);
	p_str1 = str1;
	p_str2 = str2;
	sub_check = str2;
	
	
	if (size_str1 != size_str2)
	{
		return 0;
	}
	
	while (tries_count < size_str1)
	{
		
		p_str1 = strchr(p_str1, *sub_check);
		if (NULL == p_str1)
		{
			return 0;
		}
		sub_counter = 0;
		
		
		for (i = 0; i < size_str1 && *p_str1 == *p_str2; ++i)
		{
			++p_str1;
			++p_str2;
			++sub_counter;
			
			if (p_str1 - str1  == size_str1)	
			{
				p_str1 = str1;	
			}
			if (p_str2 - str2  == size_str2)
			{
				p_str2 = str2;	
			}
		
		}
		if (sub_counter == size_str1) 
		{
			return 1;
		}
		++tries_count;
		
		++sub_check;
		
	}
	return 0;
	
}

