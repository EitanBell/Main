
#include <stddef.h> /* size_t */
#include <stdio.h> /* printf */
#include <string.h> /* strlen */

#define ASCII_SIZE (256)

void MillionChars(char c);
size_t CountSetBits(long int num);

size_t count_lut[ASCII_SIZE] = {0};

int main(void)
{
	char *str = "btb0qwetqgb4qqq0@%^#$^Q####0pEitan_Bellaiche---a$a%#&a>a<0a<";
	
	size_t str_len = strlen(str);
	size_t i = 0;
	size_t num = 123456789;
	
	for (i = 0; i < str_len; ++i)
	{
		MillionChars(str[i]);
	}
	
	printf("\nThe number of set bits in %ld is %ld\n",num, CountSetBits(num));
	
	return 0;
}

void MillionChars(char c)
{
	size_t i = 0;
	size_t max = 0;
	unsigned char max_char = '\0';
	
	if ('0' != c)
	{
		++count_lut[(size_t)c];
	}
	else
	{
		for (i = 0; i < ASCII_SIZE; ++i)
		{
			if (count_lut[i] > max)
			{
			max = count_lut[i];
			max_char = i;
			count_lut[i] = 0;
			}
		}
		printf("The max count is: %lu. The char is: %c\n", max, max_char);
	}
}

size_t CountSetBits(long int num)
{
	size_t counter = 0;
	
	while (num)
	{
		num = num & (num - 1);
		++counter;
	}
	
	return  counter;
}
















