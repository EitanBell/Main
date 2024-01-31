/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:  Moriya               *
*	Date: 07/12/22	                 *
*	Valgrind & gdb passed   	       *
*	Subject: Signle Linked List	  *
*				                 *
**************************************/
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "slist.h" /* my_slist */

typedef struct slist_node
{
	void *data;
	slist_iter_t next;
}slist_node_t;

struct slist
{
	slist_iter_t tail;
	slist_iter_t head;
};

/*------------------------------SlistCreate-----------------------------------*/

slist_t *SlistCreate(void)
{
	slist_t *slist = NULL;		/*better to name slist_handle. in code review*/
	slist_node_t *dummy = NULL;		/*changed in code review. Was: slist_iter_t dummy = NULL;*/
	
	slist = (slist_t *)malloc(sizeof(slist_t));
	if (NULL == slist)
	{
		return NULL;
	}

	dummy = (slist_iter_t)malloc(sizeof(slist_node_t));
	if (NULL == dummy)
	{
		free(slist);
		slist = NULL;
		
		return NULL;
	}
	
	dummy->data = slist;    
	dummy->next = NULL;
	
	slist->tail = dummy;
	slist->head = dummy;
	
	return slist;
}

/*------------------------------SlistDestroy---------------------------------*/

void SlistDestroy(slist_t *list)
{
	slist_iter_t iter = NULL;
	slist_iter_t tmp = NULL;
	
	assert(NULL != list);
	
	iter = list->head;
	
	while (NULL != iter)
	{
		tmp = iter;
		iter = iter->next;
		free(tmp);
		tmp = NULL;
	}
	free(list);
	list = NULL;
} 

/*------------------------------SlistInsertBefore----------------------------*/

slist_iter_t SlistInsertBefore(slist_iter_t where, void *data)
{
	slist_iter_t new_node = NULL;
	
	assert(NULL != where);
	assert(NULL != data);
    
	new_node = (slist_iter_t)malloc(sizeof(slist_node_t));
	if (NULL == new_node)
	{
		return NULL;	/*dengerous. in case of failure should return the sll end or where (Noa should update us)*/
					
	}
	
	new_node->data = where->data;
	new_node->next = where->next;
	
	if (NULL == where->next)		/*better to work with node*. work with iterators is mixed here. 
							  node* is more readable for the developer. iterator is more for the user*/
     {
	    ((slist_t *)(where->data))->tail = new_node;		/*more readable to move on external function*/
     }
	
	where->data = data;
	where->next = new_node;

	return where;
}

/*------------------------------SlistRemove---------------------------------*/

slist_iter_t SlistRemove(slist_iter_t where)
{	
	slist_iter_t tmp = NULL;
	
	assert(NULL != where);
	
	tmp = where->next;
	
	where->next = tmp->next;
	where->data = tmp->data;
	
	free(tmp);
	tmp = NULL;
	
	return where;
}

/*------------------------------SlistFind---------------------------------*/

slist_iter_t SlistFind(const slist_iter_t from, const slist_iter_t to, 
				   slist_is_match_t is_match, void *param)
{
	slist_iter_t runner = NULL;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != param);
	
	runner = from;
	
	while (to != runner && 1 != is_match((const void *)runner->data, param))
	{
		runner = runner->next;
	}
	return runner;
}

/*------------------------------SlistForEach---------------------------------*/

int SlistForEach(slist_iter_t from, slist_iter_t to, 
			     slist_do_action_t do_action, void *param)
{
	slist_iter_t runner = NULL;	/*may use a cast macro/function */
	int res_do_action = 0;		/*not necessary, we can use only from*/
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != param);
	
	runner = from;
	
	while (to != runner && 0 == res_do_action)
	{
		res_do_action = do_action(runner->data, param);
		runner = runner->next;						/*SlistNext(from)*/
	}
	return res_do_action;
}

/*------------------------------SlistGetVal---------------------------------*/

void *SlistGetVal(const slist_iter_t iter)
{
	assert(NULL != iter);	/*again, NULL should not imlement to iterator*/
	
	return (iter->data);
}

/*------------------------------SlistSetVal---------------------------------*/

slist_iter_t SlistSetVal(slist_iter_t iter, void *data)
{
	assert(NULL != iter);
	assert(NULL != data);
	
	iter->data = data;
	return iter;
}

/*------------------------------SlistCount---------------------------------*/

size_t SlistCount(const slist_t *list)
{
	size_t counter = 0;
	slist_iter_t runner_iter = NULL;
	
	runner_iter = list->head;
	while (NULL != runner_iter->next)
	{
		++counter;
		runner_iter = runner_iter->next;
	}
	return counter;
}

/*------------------------------SlistIterIsEqual---------------------------------*/

int SlistIterIsEqual(const slist_iter_t one, const slist_iter_t other)
{
	assert(one);
	assert(other);
	
	return (one == other);
}

/*------------------------------SlistIsEmpty---------------------------------*/

int SlistIsEmpty(const slist_t *list)
{
	assert(list);
	
	return (list->head == list->tail);
}

/*------------------------------SlistBegin---------------------------------*/

slist_iter_t SlistBegin(const slist_t *list)
{
	slist_iter_t iter = list->head;
	
	assert(NULL != list);
	
	return iter;
}

/*------------------------------SlistEnd---------------------------------*/

slist_iter_t SlistEnd(const slist_t *list)
{
	slist_iter_t iter = list->tail;
	
	assert(NULL != list);
	
	return iter;
}

/*------------------------------SlistAppend---------------------------------*/

slist_t *SlistAppend(slist_t *dest, slist_t *src)
{	
	assert(dest);
	assert(src);
	
	if (SlistIsEmpty(src))
	{
		return dest;
	}

	SlistEnd(dest)->data = SlistBegin(src)->data;
	SlistEnd(dest)->next = SlistBegin(src)->next;
	
	dest->tail = src->tail;
	
	SlistBegin(src)->data = src;
	SlistBegin(src)->next = NULL;
	
	src->tail = src->head;
	
	SlistEnd(dest)->data = dest;
	
	return dest;
}














