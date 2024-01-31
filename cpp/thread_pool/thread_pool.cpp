 /**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Shauli	 	   	      *
*	Date: 27/05/23	                  *
*	Subject: Thread Pool    	      *
*				                	  *
**************************************/
#include <algorithm>        // for_each
#include <vector>
#include <future>
#include <stdexcept>

#include "thread_pool.hpp"  // thread_pool API

namespace ilrd
{

/*---------------------------- ThreadPool ------------------------------------*/

ThreadPool::ThreadPool(std::size_t threadsNum): m_status(RUN),
                              m_isShutedDown(false), m_isPaused(false), m_sem(0) 
{
    for (std::size_t i = 0; threadsNum > i; ++i)
    {
        m_threads.emplace_back(std::thread(&ThreadRoutine, this));
    }
}

ThreadPool::~ThreadPool() 
{
	if (!m_isShutedDown)
    {
        Shutdown();
    }
	
    AwaitTermination();
}

/*------------------------------- Pause --------------------------------------*/

void ThreadPool::Pause(void)
{
	if (m_isShutedDown)
    {
        throw std::runtime_error("Cannot Pause() after ShutDown()");
    }
    if (m_isPaused)
    {
        throw std::runtime_error("Cannot Pause() twice");
    }

    m_status = WAIT;
    m_isPaused = true;

	for (unsigned int i = 0; i < m_threads.size(); ++i)
	{
		AddTask(std::make_shared<std::function<int(void)>>([this]()
		{
			m_sem.Wait();
			return 0;
		}), TOP_PRIORITY);
	}
}

/*------------------------------ Resume --------------------------------------*/

void ThreadPool::Resume(void)
{
	if (!m_isPaused)
    {
        throw std::runtime_error("Cannot Resume() before pause()");
    }

    for (unsigned int i = 0; m_threads.size() > i; ++i)
	{
		m_sem.Post();
	}

	m_status = RUN;
    m_isPaused = false;		
}

/*------------------------------ Shutdown ------------------------------------*/

void ThreadPool::Shutdown(void)
{
	if (m_isShutedDown)
    {
		throw std::runtime_error("Cannot Shutdown Twice");
    }

    ThreadPool::ThreadStop();
    
    m_isShutedDown = true;
}

/*---------------------------- AwaitTermination ------------------------------*/

void ThreadPool::AwaitTermination(void)
{
	if (!m_isShutedDown)
    {
		throw std::runtime_error("Cannot AwaitTermination() before Shutdown()");
    }

    for (size_t i = 0; m_threads.size() > i; ++i)
	{
		if (m_threads[i].joinable())
		{
			m_threads[i].join();
		}
	}
}

/*------------------------------- AddTask ------------------------------------*/

ThreadPool::TaskFuture ThreadPool::AddTask(std::shared_ptr<std::function
                                           <int(void)>> func, Priority priority)
{
    std::shared_ptr<Task> newTask(new Task(func, priority));
	m_tasksQueue.Push(newTask);

	return (TaskFuture(newTask));
}

/*---------------------------- ThreadRoutine ---------------------------------*/

void ThreadPool::ThreadRoutine(ThreadPool *tp) 
{
    while (RUN == tp->m_status)
    {
        std::shared_ptr<Task> task;
        tp->m_tasksQueue.Pop(task);
        task->Run();
    }
}

/*---------------------------- ThreadStop ------------------------------------*/

void ThreadPool::ThreadStop(void)
{
	if (DONE == m_status)
	{
		return;
	}

	for (size_t i = 0; i < m_threads.size(); ++i)
	{
		AddTask(std::make_shared<std::function<int(void)>>([this] 
		{
			m_status = DONE; 
			return 0;
		}), LEAST_PRIORITY);
	}
}

/*-------------------------------- TaskFuture --------------------------------*/

ThreadPool::TaskFuture::TaskFuture(std::shared_ptr<Task> task): m_task(task) {}

ThreadPool::TaskFuture::~TaskFuture() {}

int ThreadPool::TaskFuture::WaitResult(std::chrono::milliseconds timeout) const
{
    return (m_task->WaitResult(timeout));
}

ThreadPool::Status ThreadPool::TaskFuture::GetStatus() const
{
    return (m_task->GetStatus());
}

/*---------------------------------- Task ------------------------------------*/

ThreadPool::Task::Task(std::shared_ptr<std::function<int(void)>> func, 
                              Priority priority) :m_func(func), m_sem_task(0UL), 
                               m_priority(priority), m_result(0), m_status(WAIT)
{
}

ThreadPool::Task::~Task() {}

bool ThreadPool::Task::operator<(const Task& other) const
{
    return (m_priority > other.m_priority);
}

void ThreadPool::Task::Run()
{
	m_status = RUN;
	try
	{
		(*m_func)();
	}
	catch(const std::exception& e)
	{
		m_result = -1;
	}

	m_status = DONE;
	m_result = 0;
	m_sem_task.Post();
}

bool ThreadPool::FunctorTaskLessThan::operator()(const std::shared_ptr<Task>& 
                                task1, const std::shared_ptr<Task>& task2) const
{
    return (*task1 < *task2);
}

int ThreadPool::Task::WaitResult(std::chrono::milliseconds timeout) const
{
    return (m_sem_task.Wait(timeout));
}

ThreadPool::Status ThreadPool::Task::GetStatus() const
{
    return m_status;
}

} //namespace ilrd