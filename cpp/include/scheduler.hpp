/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Gal		              *
*	Date: 18/06/23	                  *
*	Subject: scheduler		   		  *
*				                	  *
**************************************/	

#ifndef ILRD_RD136_7_SCHEDULER_HPP
#define ILRD_RD136_7_SCHEDULER_HPP

#include <functional>           // std::function<>
#include <chrono>               // std::chrono  
#include <memory>               // shared ptr
#include <thread>               // thread
#include <mutex>                // mutex
#include <condition_variable>   // condition variable

#include "handleton.hpp"        // ilrd::handleton<>
#include "waitable_queue.hpp"   // waitable queue
#include "priority_queue.hpp"   // priority queue

/******************************** Documentation ********************************

- The scheduler starts running from the Constructor.
- Stop() stops executing tasks.
- Use Run() only after Stop(), to re-run the tasks execution.

NOTE!
    - when std::function returns "false" - it means that the task should re-run.
    - In the destructor, all unfinished tasks will be executed, unless Stop() 
      has been called before.
    - Stop() within a task will lead to undifiend behavior.
    - Adding tasks during the shutdown is not allowed.

*******************************************************************************/

namespace ilrd
{

/****************************** class Scheduler *******************************/

class Scheduler
{
    
public:
    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;

    void AddTask(std::shared_ptr<std::function<bool(void)>> callback,
                     std::chrono::milliseconds interval); // false = re-run task
    void Run();
    void Stop();
        
private:
    class Task;
    
    Scheduler();
    ~Scheduler();

    void ThreadRoutine();
    void AwakeThread();
    
    static const constexpr int MAX_RERUNS = 3;
    friend class Handleton<Scheduler>;

    class TaskFunctorLessThan
    {
    public:
        bool operator()(const std::shared_ptr<Task>& task1, 
                        const std::shared_ptr<Task>& task2) const;
    };

    WaitableQueue<PriorityQueue<std::shared_ptr<Task>,
              std::vector<std::shared_ptr<Task>>, TaskFunctorLessThan>> m_tasks;
    std::thread m_thread;
    std::mutex m_mutex;
    std::condition_variable m_condVar;
    bool m_isRunning;
    bool m_RunFlag;
    bool m_isShutdown;
};

/*************************** class Scheduler::Task ****************************/

class Scheduler::Task
{
public:
    Task(std::shared_ptr<std::function<bool(void)>> callback,
                                            std::chrono::milliseconds interval);
    bool Run();
    std::chrono::time_point<std::chrono::system_clock> GetRunTime() const;
    bool operator<(const Task& other) const;
    
private:
    size_t m_runCount;
    const std::chrono::milliseconds m_interval;
    std::chrono::time_point<std::chrono::system_clock> m_runTime;  // absolute time to run task
    std::shared_ptr<std::function<bool(void)>> m_callback;
};

}   // namespace ilrd

#endif /* ILRD_RD136_7_SCHEDULER_HPP */  