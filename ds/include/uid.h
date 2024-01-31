/*************************************
*				   			  *
*	Developer: Eitan Bellaiche      *	 
*	Reviewer:  Matan                *
*	Date: 01/01/23	                 *
*	Valgrind & gdb passed   	       *
*	Subject: UID	  			  *
*				                 *
**************************************/

#ifndef __UID_H__
#define __UID_H__

#include <sys/types.h>	/*pid_t*/
#include <time.h> /*time_t*/

/*------------------------------------------------------------------------------
UID:
	An UID is a Unique IDentifier which associated with a single entity
    ד within a given system.

MAIN FUNCTIONALITY:
	UIDGenerate, UIDIsEqual;
------------------------------------------------------------------------------*/

typedef struct ilrd_uid ilrd_uid_t;

extern const ilrd_uid_t bad_uid;

struct ilrd_uid
{
   pid_t pid;
   time_t time;
   size_t counter;
};

/**
 * UIDGenerate Craete a new UID.
 * @return A new UID, or bad_uid in case of failure.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
ilrd_uid_t UIDGenerate(void);

/**
 * UIDIsEqual checks if two UIDs are equel.
 * @param handle1: first UID to compare.
 * @param handle2: second UID to compare.
 * @return 1 if the UIDs are equal, 0 if not.
 * @time_complexity O(1)
 * @space_complexity O(1)
 */
int UIDIsEqual(ilrd_uid_t handle1, ilrd_uid_t handle2);

#endif /* __UID_H__ */

