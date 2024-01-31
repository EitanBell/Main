#include <string.h> /* strlen, strncap */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */
#include <stddef.h> /* size_t */
#include <stdio.h> /* printf */

#define NULL_TERM (1)

char *FlipWords(char *str);

int main(void)
{
	char *str = "i am student";
	char *str_flipped = NULL;
	
	str_flipped = FlipWords(str);
	
	printf("%s\n", str_flipped);
	
	free(str_flipped);
	str_flipped = NULL;
	
	return 0;
}

char *FlipWords(char *str)
{
	size_t size = 0;
	size_t counter = 0;
	char *runner = NULL;
	char *strcopy = NULL;
	
	assert(NULL != str);
	
	size = strlen(str);
	strcopy = (char *)malloc(sizeof(char) * size);
	if (NULL == strcopy)
	{
		return NULL;	
	}
	runner = str + size - NULL_TERM;
	
	while (runner > str)
	{
		if (' ' == *runner || runner == str)
		{
			strncat(strcopy, runner + 1, counter);
			strncat(strcopy, " ", 1);
			counter = 0;
		}
		--runner;
		++counter;
	}
	
	strncat(strcopy, runner - 1, counter);

	*(strcopy + size - NULL_TERM) = '\0';
	return strcopy;
}
















