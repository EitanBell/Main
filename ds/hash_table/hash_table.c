/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Gal	                  *
*	Date: 05/02/23	                  *
*	Subject: Hash Table 		 	  *
*				                	  *
**************************************/

#include <stdlib.h>		/* calloc */
#include <assert.h>		/* assert */

#include "dlist.h"		/* doubly linked list API */
#include "hash_table.h"		/* hash_table API */

enum status
{
	SUCCESS = 0,
	FAIL
};

enum boolean
{
	FALSE = 0,
	TRUE
};

struct hash 
{
	dlist_t **table;
	size_t table_size;
	hash_get_key_t get_key;
	hash_func_t hash_func;
	hash_is_match_t is_match;
};

/*------------- Helper Funcs -------------*/
static void InitHash(hash_t *hash, size_t table_size, 
		 hash_get_key_t GetKey, hash_func_t hash_func, hash_is_match_t IsMatch);
static dlist_iter_t FindIter(dlist_t *list, const hash_t *hash,
		 													   
		 													   const void *key);
/*----------------------------------------------------------------------------*/
/*                                MAIN FUNCTIONS                              */
/*----------------------------------------------------------------------------*/		
		 			 
/*------------------------------- HashCreate ---------------------------------*/

hash_t *HashCreate(hash_get_key_t GetKey, hash_is_match_t IsMatch, 
								       hash_func_t hash_func, size_t table_size)
{
	hash_t *hash = NULL;
	
	assert(NULL != GetKey);
	assert(NULL != IsMatch);
	assert(NULL != hash_func);
	assert(0 < table_size);
	
	hash = (hash_t *)calloc(1, sizeof(hash_t) + table_size * sizeof(dlist_t *));
	if (NULL == hash)
	{
		return NULL;
	}
	
	hash->table = (dlist_t **)(hash + 1);
	InitHash(hash, table_size, GetKey, hash_func, IsMatch);
	
	return hash;
}

/*------------------------------- HashDestroy --------------------------------*/

void HashDestroy(hash_t *hash)
{
	size_t i = 0;
	
	assert(NULL != hash);
	
	for (i = 0; hash->table_size > i; ++i)
	{
		if (NULL != hash->table[i])
		{
			DlistDestroy(hash->table[i]);
		}
	}
	
	free(hash);
	hash = NULL;
}

/*------------------------------- HashInsert --------------------------------*/	

int HashInsert(hash_t *hash, void *item)
{
	void *key = NULL;
	size_t index = 0;
	
	assert(NULL != hash);
	
	key = hash->get_key(item);
	index = hash->hash_func(key);
	
	if (NULL == hash->table[index])
	{
		hash->table[index] = DlistCreate();
		if (NULL == hash->table[index])
		{
			return FAIL;
		}
	}
	
	return DlistIterIsEqual(DlistEnd(hash->table[index]), 
							DlistPushBack(hash->table[index], item));
}


/*------------------------------- HashRemove --------------------------------*/

void HashRemove(hash_t *hash, const void *key)
{
	size_t index = 0;
	dlist_t *list = NULL;
	dlist_iter_t item_iter = NULL;

	assert(NULL != hash);
	
	index = hash->hash_func(key);
	list = hash->table[index];
	if (NULL == list)
	{
		return;
	}
	
	item_iter = FindIter(list, hash, key);
	if (!DlistIterIsEqual(item_iter, DlistEnd(list)))
	{
		DlistRemove(item_iter);
	}
}

/*-------------------------------- HashFind ----------------------------------*/

void *HashFind(const hash_t *hash, const void *key)
{
	size_t index = 0;
	dlist_iter_t item_iter = NULL;
	dlist_t *list = NULL;
	
	assert(NULL != hash);
	assert(NULL != key);
	
	index = hash->hash_func(key);
	list = hash->table[index];
	if (NULL == list)
	{
		return NULL;
	}
	
	item_iter = FindIter(list, hash, key);
	if (DlistIterIsEqual(item_iter, DlistEnd(list)))
	{
		return NULL;
	}

	return DlistGetVal(item_iter);
}


/*------------------------------ HashForeach ---------------------------------*/

int HashForeach(hash_t *hash, hash_callback_t callback, void *param)
{
	size_t i = 0;
	int status = SUCCESS;
	
	assert(NULL != hash);
	assert(NULL != callback);
	
	for (i = 0; (hash->table_size > i) && (SUCCESS == status); ++i)
	{
		dlist_t *list = hash->table[i];
		if (NULL != list)
		{	
			status = DlistForEach(DlistBegin(list), DlistEnd(list), 
															   callback, param);
		}
	}
	
	return status;
}


/*-------------------------------- HashSize ----------------------------------*/

size_t HashSize(const hash_t *hash)
{
	size_t counter = 0;
	size_t i = 0;
	
	assert(NULL != hash);
	
	for (i = 0; hash->table_size > i; ++i)
	{
		dlist_t *list = hash->table[i];
		if (NULL != list)
		{
			counter += DlistCount(list);
		}
	}
	
	return counter;
}

/*------------------------------- HashIsEmpty --------------------------------*/

int HashIsEmpty(const hash_t *hash)
{
	size_t i = 0;
	int status = TRUE;
	
	assert(NULL != hash);
	
	for (i = 0; (hash->table_size > i) && (TRUE == status); ++i)
	{
		dlist_t *list = hash->table[i];
		if (NULL != list)
		{
			status = DlistIsEmpty(list);
		}
	}
	
	return status;
}

/*----------------------------------------------------------------------------*/
/*                                HELPER FUNCTIONS                            */
/*----------------------------------------------------------------------------*/

/*------------------------------ InitHashTable -------------------------------*/

static void InitHash(hash_t *hash, size_t table_size, hash_get_key_t GetKey, 
								 hash_func_t hash_func, hash_is_match_t IsMatch)
{
	size_t i = 0;
	assert(NULL != hash);
	assert(NULL != GetKey);
	assert(NULL != IsMatch);
	assert(NULL != hash_func);

	hash->table_size = table_size;
	hash->get_key = GetKey;
	hash->hash_func = hash_func;
	hash->is_match = IsMatch;
	
	for (i = 0; hash->table_size > i; ++i)
	{
		hash->table[i] = NULL;
	}
}

static dlist_iter_t FindIter(dlist_t *list, const hash_t *hash, const void *key)
{
	dlist_iter_t runner = 0;
	
	assert(NULL != hash);
	assert(NULL != list);
	
	runner = DlistBegin(list);
	
	while ((!DlistIterIsEqual(runner, DlistEnd(list))) && 
			!hash->is_match(key, hash->get_key(DlistGetVal(runner))))
	{
		runner = DlistNext(runner);
	}
	
	return runner;
}


 
