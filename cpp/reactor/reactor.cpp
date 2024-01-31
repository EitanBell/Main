/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Ariel		              *
*	Date: 16/06/23	                  *
*	Subject: reactor		   		  *
*				                	  *
**************************************/		

#include <iostream>			// std::cerr
#include <sys/select.h>		// select 
#include <algorithm>		// for_each

#include "reactor.hpp"		// reactor API

namespace ilrd
{
/************************** Implementation Reactor ****************************/

Reactor::Reactor(): m_isRunning(false) {}

void Reactor::Add(int fd, Mode mode, const std::function<void(void)>& callback)
{
	fd_mode_t fdMode(fd, mode);

	if (m_callbacks.end() == m_callbacks.find(fdMode))
	{
		m_fdVec.push_back(fdMode);
	}

	m_callbacks[fdMode] = callback;
}

void Reactor::Remove(int fd, Mode mode)
{
	fd_mode_t fdModeToRemove(fd, mode);

	try
	{
		m_callbacks.at(fdModeToRemove);
	}
	catch(const std::out_of_range& e)
	{
		throw std::invalid_argument("Argument Is not In Reactor");
	}

	auto iterToRemove = std::find(m_fdVec.begin(), m_fdVec.end(), 
                                                                fdModeToRemove);
	m_fdVec.erase(iterToRemove);
	m_callbacks.erase(fdModeToRemove);
} 

void Reactor::Run()
{
	if (m_isRunning)
	{
		return;
	}
	
	m_isRunning = true;
	FDListener listener(1);

	while (m_isRunning)
	{
		std::vector<fd_mode_t> readyFds = std::move(listener.Wait(m_fdVec));
		
        InvokeFuncUntilSTOP(readyFds);
	}
}

void Reactor::Stop()
{
	m_isRunning = false;
}

/************************* Implementation FDListener **************************/

Reactor::FDListener::FDListener(int timeoutSec): m_timeoutSec(timeoutSec) {}

std::vector<std::pair<int, Reactor::Mode>> Reactor::FDListener::
									   Wait(const std::vector<fd_mode_t>& fdVec)
{
	fd_set fdModeSets[NUM_OF_MODES];

	FD_ZERO(&fdModeSets[READ]);
	FD_ZERO(&fdModeSets[WRITE]);
	FD_ZERO(&fdModeSets[EXCEPT]);

	int maxFd = 0;
	std::for_each(fdVec.begin(), fdVec.end(), 
	[&fdModeSets, &maxFd](const fd_mode_t& curr_fd)
	{
		FD_SET(curr_fd.first, &fdModeSets[curr_fd.second]);
		if (maxFd < curr_fd.first)
		{
			maxFd = curr_fd.first;
		}
	});	

	timeval timeout = {m_timeoutSec, 0};

	int status  = select(maxFd + 1, &fdModeSets[READ], &fdModeSets[WRITE], 
						&fdModeSets[EXCEPT], &timeout);
	if (-1 == status)
	{
		std::__throw_runtime_error("Select Failed");
	}
	
	std::vector<fd_mode_t> readyFds;
	if (0 != status)
	{
		std::for_each(fdVec.begin(), fdVec.end(), 
	[&fdModeSets, &readyFds](const fd_mode_t& curr_fd)
	{
		if (FD_ISSET(curr_fd.first, &fdModeSets[curr_fd.second]))
		{
			readyFds.push_back(curr_fd);
		}
	});	
	}

	return readyFds;	
}

/****************************** Helper Functions ******************************/

void Reactor::InvokeFuncUntilSTOP(std::vector<fd_mode_t>& fdVec)
{
    std::find_if_not(fdVec.begin(), fdVec.end(), 
        [this, &fdVec](const fd_mode_t& curr_fd)
        {
            if (m_callbacks.end() != m_callbacks.find(curr_fd))
            {
                try
                {
                    m_callbacks[curr_fd]();
                }
                catch(const std::exception& e)
                {
                }
            }
            return m_isRunning;
        });	
}


}   // namespace ilrd