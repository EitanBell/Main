 /**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Shauli	 	   	      *
*	Date: 27/05/23	                  *
*	Subject: Thread Pool    	      *
*				                	  *
**************************************/
#ifndef ILRD_RD136_7_THREAD_POOL_HPP
#define ILRD_RD136_7_THREAD_POOL_HPP

#include <thread>   // hardware_concurrency
#include <functional> // function

#include "semaphore.hpp"      // Semaphore API
#include "priority_queue.hpp" // PriorityQueue API
#include "waitable_queue.hpp" // WaitableQueue API

namespace ilrd
{

/*-------------------------------- ThreadPool --------------------------------*/

class ThreadPool
{
public:
    class TaskFuture;

    enum Status : int
    {
        WAIT = 0,
        RUN,
        DONE
    };

    enum Priority : int
    {
        HIGH = 0,
        MID,
        LOW
    };

    explicit ThreadPool(std::size_t threadsNum = 
                                           std::thread::hardware_concurrency());
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ~ThreadPool();

    TaskFuture AddTask(std::shared_ptr<std::function<int(void)>> func, 
                                                             Priority priority);
    void Pause();
    void Resume();
    void Shutdown();
    void AwaitTermination(); 

private:
    class Task;

    class FunctorTaskLessThan
    {
    public:
        bool operator()(const std::shared_ptr<Task>& task1, 
                                      const std::shared_ptr<Task>& task2) const;
    };
    
    std::vector<std::thread> m_threads;
    WaitableQueue<PriorityQueue<std::shared_ptr<Task>, 
         std::vector<std::shared_ptr<Task>>, FunctorTaskLessThan>> m_tasksQueue;
    Status m_status;
    bool m_isShutedDown;
    bool m_isPaused;
    Semaphore m_sem;

    static void ThreadRoutine(ThreadPool *treadPool);
    void ThreadStop(void);

    const static Priority TOP_PRIORITY = static_cast<Priority>(HIGH - 1);
    const static Priority LEAST_PRIORITY = static_cast<Priority>(LOW + 1);
};

/*-------------------------- ThreadPool::TaskFuture --------------------------*/

class ThreadPool::TaskFuture
{
public:
    explicit TaskFuture(std::shared_ptr<Task> task);
    ~TaskFuture();

    int WaitResult(std::chrono::milliseconds timeout = 
                                    std::chrono::milliseconds(INT32_MAX)) const;
    Status GetStatus() const;

private:
    std::shared_ptr<Task> m_task;
};

/*---------------------------- ThreadPool::Task ------------------------------*/

class ThreadPool::Task
{
public:
    explicit Task(std::shared_ptr<std::function<int(void)>> func, 
                                                             Priority priority);
    ~Task();
    
    bool operator<(const Task& other) const;
    void Run();
    int WaitResult(std::chrono::milliseconds timeout = 
                                    std::chrono::milliseconds(INT32_MAX)) const;
    Status GetStatus() const;

private:
    std::shared_ptr<std::function<int(void)>> m_func;
    mutable Semaphore m_sem_task;
    Priority m_priority;
    int m_result;
    Status m_status;
};

} //namespace ilrd

#endif /* ILRD_RD136_7_THREAD_POOL_HPP */   
