/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Gal	                  *
*	Date: 05/02/23	                  *
*	Subject: Hash Table 		 	  *
*				                	  *
**************************************/

#include <stdio.h>	/* printf */
#include <string.h>	/* strcmp */
#include <stdlib.h>	/* malloc */

#include "hash_table.h"	/* hash_table API */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")
#define TABLE_SIZE 10
#define TABLE_SIZE_2 ((size_t)5381)
					   			        
typedef struct item
{
	size_t key;
	char value;
} item_t;

enum status
{
	SUCCESS = 0,
	FAIL
} status_t;

/*------------- FUNC DECLARATIONS -------------*/
void RunTests(void);

size_t HashFunc(const void *key);
int IsMatch(const void *key1, const void *key2);
void *GetKey(const void *item);
int Print(void *item, void *param);

size_t HashFunc2(const void *key);
int IsMatch2(const void *key1, const void *key2);
void *GetKey2(const void *item);
int Clean(void *item, void *param);

int IntegraionTest(void);
void SpellCheckerTest(void);

/*------------- MAIN FUNC -------------*/
int main(void)
{
	RunTests();
	
	return 0;
}

/*------------- FUNC INPLEMENTATIONS -------------*/
void RunTests(void)
{
	printf("\n\t******************** TESTS BEGIN *******************\n\n");
	
	printf("\t\tHashCreate   Test: \t%s\n", TEST_RES(IntegraionTest()));
	printf("\t\tHashDestroy  Test: \t%s\n", TEST_RES(IntegraionTest()));
	printf("\t\tHashInsert   Test: \t%s\n", TEST_RES(IntegraionTest()));
	printf("\t\tHashRemove   Test: \t%s\n", TEST_RES(IntegraionTest()));
	printf("\t\tHashFind     Test: \t%s\n", TEST_RES(IntegraionTest()));
	printf("\t\tHashSize     Test: \t%s\n", TEST_RES(IntegraionTest()));
	printf("\t\tHashIsEmpty  Test: \t%s\n", TEST_RES(IntegraionTest()));
	printf("\t\tHashForEach  Test: \t%s\n", TEST_RES(IntegraionTest()));
	printf("\t\tSpellChecker Test:\n");
	SpellCheckerTest();
	
	printf("\n\t****************** TESTS FINISHED ******************\n\n");
	
}

int IntegraionTest(void)
{
	hash_t *hash = HashCreate(&GetKey, &IsMatch, &HashFunc, TABLE_SIZE);
	item_t arr[26] = {0};
	size_t len = sizeof(arr) / sizeof(*arr);
	size_t i = 0;
	int empty_status = 0;
	int empty2_status = 0;
	int size_status = 0;
	int size2_status = 0;
	int insert_status = 0;
	int find_status = 0;
	int remove_status = 0;
	size_t key = 0;
	
	for (i = 0; i < len; ++i)
	{
		item_t item = {0};
		item.key = i + 1;
		item.value = 'a' + i;
		arr[i] = item;
	}
	
	empty_status = HashIsEmpty(hash) ? SUCCESS : FAIL;
	size_status = (0 == HashSize(hash)) ? SUCCESS : FAIL;
	
	for (i = 0; (i < len) && (0 == insert_status); ++i)
	{
		insert_status = HashInsert(hash, arr + i);
	}
	
	empty2_status = !HashIsEmpty(hash) ? SUCCESS : FAIL;
	size2_status = (len == HashSize(hash)) ? SUCCESS : FAIL;
	
	find_status = 1;
	for (i = 0; (i < len) && find_status; ++i)
	{
		key = i + 1;
		find_status = (((item_t *)HashFind(hash, &key))->value ==
					  (char)('a' + i)) ? SUCCESS : FAIL;
	}
	
	key += 10 ;
	find_status = (NULL == HashFind(hash, &(key)));
	
	for (i = 0; (i < len / 2); ++i)
	{
		size_t key = i + 1;
		HashRemove(hash, &key);
	}
	
	HashRemove(hash, &(arr[0].key));
	remove_status += !HashIsEmpty(hash) ? SUCCESS : FAIL;
	remove_status += (len / 2 == HashSize(hash)) ? SUCCESS : FAIL;
	
	/*-------------- Activate for ForEach Test --------------*/
/*	HashForeach(hash, &Print, NULL);*/
/*	printf("\n");*/
	
	HashDestroy(hash);
	hash = NULL;

	return empty_status + empty2_status + size_status + size2_status +
		   insert_status + find_status + remove_status;
}

void SpellCheckerTest(void)
{
	hash_t *hash = HashCreate(&GetKey2, &IsMatch2, &HashFunc2, TABLE_SIZE_2);
	FILE *fp = NULL;
	char temp[64] = {0};
	char input[64] = {0};
	int to_quit = 0;
	
	if (NULL == (fp = fopen("/usr/share/dict/words", "r")))
	{
		printf("Can't read the file.\n");
	}
	
	while (1 == fscanf(fp, "%s", temp))
	{
		char *item = (char *)malloc(strlen(temp) + 1);
		strcpy(item, temp);
		HashInsert(hash, item);
	}
	
	while(!to_quit)
	{
		printf("\t\tEnter a word (or press 'q' to quit):  ");
		scanf("%s", input);
		
		to_quit = (0 == strcmp("q", input));

		(NULL != HashFind(hash, input)) ? 
			 printf("\t\tThe word\033[91m is in \033[0mthe dictionary\n\n") : 
			 printf("\t\tThe word\033[91m is not in \033[0mthe dictionary\n\n");	
	}
		
	HashForeach(hash, &Clean, NULL);
		
	fclose(fp);
	
	HashDestroy(hash);
	hash = NULL;
}

size_t HashFunc(const void *key)
{
	return (*(size_t *)key % TABLE_SIZE);
}


int IsMatch(const void *key1, const void *key2)
{
	return (*(size_t *)key1 == *(size_t *)key2);
}

void *GetKey(const void *item)
{
	return (&(((item_t *)item)->key));
}

int Print(void *item, void *param)
{
	printf("%c, ", ((item_t *)item)->value);
	(void)param;
	
	return 0;
}

size_t HashFunc2(const void *key)
{
	size_t hash = TABLE_SIZE_2;
	int c = 0;
	unsigned char *str = (unsigned char *)key;

	while (0 != (c = *str++))
	{
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}

	return (hash % TABLE_SIZE_2);
}

int IsMatch2(const void *key1, const void *key2)
{
	return (0 == strcmp((char *)key1, (char *)key2));
}

void *GetKey2(const void *item)
{
	return ((void *)item);
}

int Clean(void *item, void *param)
{
	(void)param;
	
	free(item);
	item = NULL;
	
	return 0;
}

