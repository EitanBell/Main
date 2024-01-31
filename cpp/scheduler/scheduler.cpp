/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Gal		              *
*	Date: 18/06/23	                  *
*	Subject: scheduler		   		  *
*				                	  *
**************************************/		
#include <stdexcept>     // runtime_error

#define I_AM_THE_HANDLETON_IMPLEMENTER
#include "scheduler.hpp"	// scheduler API

INSTANTIATE_HANDLETON(ilrd::Scheduler)
namespace ilrd
{
/************************* Implementation scheduler ***************************/

Scheduler::Scheduler(): m_thread(&Scheduler::ThreadRoutine, this), 
			 	      m_isRunning(true), m_RunFlag(false), m_isShutdown(false){}

Scheduler::~Scheduler()
{
    m_isShutdown = true;
    Stop();
}

void Scheduler::AddTask(std::shared_ptr<std::function<bool(void)>> callback,
                 							 std::chrono::milliseconds interval)
{
    if (m_isShutdown)
    {
        throw std::runtime_error("Cannot Add tasks during shutdown");
    }
    
    m_tasks.Push(std::make_shared<Task>(Task(callback, interval)));

    AwakeThread();	
}

void Scheduler::Run()
{
    if (!m_isRunning)
    {
        m_isRunning = true;

        m_thread = std::thread(&Scheduler::ThreadRoutine, this);
    }
}

void Scheduler::Stop()
{
    m_isRunning = false;
    
    m_tasks.Push(std::make_shared<Task>(Task 
				(std::make_shared<std::function<bool(void)>>
				(std::function<bool(void)>([]()
    {
        return true;
    })), std::chrono::milliseconds(0))));

    AwakeThread();

    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void Scheduler::ThreadRoutine()
{
    while (m_isRunning || (m_isShutdown && (!m_tasks.IsEmpty())))
    {
        std::shared_ptr<Scheduler::Task> currTask(0);
        m_tasks.Pop(currTask);
        
        std::chrono::time_point<std::chrono::system_clock> runTime =
                                                         currTask->GetRunTime();
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_condVar.wait_until(lock, runTime,[this]()
            {
                return m_RunFlag;
            });
        }

        m_RunFlag = false;
        
        if ((currTask->GetRunTime() > std::chrono::system_clock::now()) ||
            												   !currTask->Run())
        {
            m_tasks.Push(currTask);
        }
    }
}

void Scheduler::AwakeThread()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_RunFlag = true;
    m_condVar.notify_one();
}

/**************************** Implementation Task *****************************/

Scheduler::Task::Task(std::shared_ptr<std::function<bool(void)>> callback,
	std::chrono::milliseconds interval): 
	m_runCount(0),
	m_interval(interval),
	m_runTime(std::chrono::system_clock::now() + interval),
	m_callback(callback) {}

bool Scheduler::Task::Run()
{
    bool isDone = (*m_callback)();
    ++m_runCount;

    if (MAX_RERUNS == m_runCount)
    {
        isDone = true;
    }
    else if (!isDone)
    {
        m_runTime = std::chrono::system_clock::now() + m_interval;
    }

    return isDone;
}

std::chrono::time_point<std::chrono::system_clock>
                                            Scheduler::Task::GetRunTime() const
{
    return m_runTime;
}

bool Scheduler::Task::operator<(const Task& other) const
{
    return (m_runTime > other.m_runTime);
}

/****************************** Helper Functions ******************************/

bool Scheduler::TaskFunctorLessThan::operator()
                                      (const std::shared_ptr<Task>& task1, 
                                       const std::shared_ptr<Task>& task2) const
{
    return (*task1 < *task2);
}


}   // namespace ilrd