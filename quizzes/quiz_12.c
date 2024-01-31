#include <string.h> /* strlen, strchr, strncpy*/
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
	char *runner = NULL;
	char *buffer = NULL;
	
	assert(NULL != str);
	
	size = strlen(str);
	buffer = (char *)malloc(sizeof(char) * size);
	if (NULL == buffer)
	{
		return NULL;	
	}
	buffer += size;
	runner = str + size - NULL_TERM;
	
	
	while (NULL != strchr(str, ' '))
	{
		runner = strchr(str, ' ');
		buffer -= (runner - str);
		buffer = strncpy(buffer, str, runner - str);
		--buffer;
		*buffer = ' ';
		str = runner + 1;
	}
	
	runner = strchr(str, '\0');
	buffer -= (runner - str);
	buffer = strncpy(buffer, str, runner - str);
	
	return buffer;
}
















