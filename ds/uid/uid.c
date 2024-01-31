/*************************************
*				   			 		 *
*	Developer: Eitan Bellaiche       *	 
*	Reviewer:  Matan                 *
*	Date: 01/01/23	                 *
*	Valgrind & gdb passed   	     *
*	Subject: UID	  		   	     *
*				                	 *
**************************************/

#include <unistd.h> /* getpid */
#include <stdatomic.h> /* atomic funcs */

#include "uid.h" /* uid */

const ilrd_uid_t bad_uid = {0, -1, 0};

static size_t GetCounter(void);

/*------------------------------ UIDGenerate ---------------------------------*/

ilrd_uid_t UIDGenerate(void)
{
	ilrd_uid_t uid = {0};
	
	uid.time = time(NULL);
	if ((time_t)-1 == uid.time)
	{
		uid = bad_uid;
	}
	else
	{
		uid.pid = getpid();
		uid.counter = GetCounter();
	}
		
	return uid;
}

/*------------------------------ UIDIsEqual ---------------------------------*/

int UIDIsEqual(ilrd_uid_t handle1, ilrd_uid_t handle2)
{
	return ((handle1.pid == handle2.pid) && (handle1.time == handle2.time)
	         && (handle1.counter == handle2.counter));
}

/*------------------------------HELPER FUNCTIONS------------------------------*/

/*-------------------------------- GetCounter --------------------------------*/

static size_t GetCounter(void)
{
	static atomic_size_t uid_counter = 0;
	
	atomic_fetch_add(&uid_counter, 1);
	
	return (atomic_load(&uid_counter));
}


