

#include <stdio.h>		/* printf */
#include <assert.h>		/* assert */
#include <stdlib.h> 	/* malloc, calloc */
#include <stddef.h> 	/* size_t */
#include <string.h>		/* strlen */

void PrintStrPermutations(char* str, size_t len, size_t *already_used_lut, 
										  char* perm_str, size_t pos, size_t i);

int main (void)
{    
    char *str = {"Eitan"};
    size_t len = strlen(str);
    size_t i = 0;
    char *perm_str = NULL;
    size_t *already_used_lut = NULL;
    
    perm_str = (char *)malloc((2 * len) * sizeof(char));
    if (NULL == perm_str)
    {
    	return 0;
    }
    
    already_used_lut = (size_t *)calloc(len, sizeof(size_t));
    if (NULL == already_used_lut)
    {
    	free(perm_str);
    	perm_str = NULL;
    	
    	return 0;
    }

    printf("All permutations:\n");
    PrintStrPermutations(str, len, already_used_lut, perm_str, 0, i);

    return 0;
}


void PrintStrPermutations(char* str, size_t len, size_t *already_used_lut, 
										   char* perm_str, size_t pos, size_t i)
{	
	assert(NULL != str);
	assert(NULL != perm_str);
	assert(0 < len);
	
	if (pos == len) {
        printf("%s\n", perm_str);
        return;
    }
    
	for (i = 0; i < len; i++) {
    	if (!already_used_lut[i]) 
        {
            already_used_lut[i] = 1;
            perm_str[pos] = str[i];
            PrintStrPermutations(str, len, already_used_lut, perm_str, pos + 1, 
																			 i);
            already_used_lut[i] = 0;
        }
    }	
}

