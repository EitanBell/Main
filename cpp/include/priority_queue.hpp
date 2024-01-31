/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: 	                      *
*	Date: 24/05/23	                  *
*	Subject: Priority Queue   		  *
*				                	  *
**************************************/

#ifndef ILRD_RD136_7_PRIORITY_QUEUE_HPP
#define ILRD_RD136_7_PRIORITY_QUEUE_HPP

#include <queue>
#include <vector>			//std::vector
#include <functional> 		//std::less

namespace ilrd
{
template<class T, class Container = std::vector<T>, class Compare = 
									  std::less<typename Container::value_type>>
class PriorityQueue: private std::priority_queue<T, Container, Compare>
{
    typedef std::priority_queue<T, Container, Compare> pq_t;

public:
    typedef typename pq_t::value_type value_type;
    typedef typename pq_t::reference reference;

    using pq_t::push;
    using pq_t::pop;
    using pq_t::empty;
    using pq_t::top;

    inline const typename pq_t::value_type& front() const
    {
        return this->top();
    }
};

} //ilrd

#endif	// ILRD_RD136_7_PRIORITY_QUEUE_HPP
