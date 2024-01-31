/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Date: 14/07/23                    *
*	Subject: Framework v0.4 		  *
**************************************/

#ifndef ILRD_RD136_7_FRAMEWORK_V_0_4_HPP
#define ILRD_RD136_7_FRAMEWORK_V_0_4_HPP

/*
THIS VERSION (V0.4) INCLUDES REACTOR, THREADPOOL, LOGGER, FACTORY, PLUGINS 
AND ASYNC INTEGRATION 
*/

#include <functional>      // std::function<>
#include <thread>          // threads
#include <vector>          // vector
#include <tuple>           // tuple
#include <filesystem>      // filesystem

#include "msg.hpp"                  // Msg (abstract) API, Mode enum
#include "reactor.hpp"              // Reactor API
#include "thread_pool.hpp"          // ThreadPool API
#include "logger.hpp"               // Logger API
#include "factory_singleton.hpp"    // Factory Singleton API
#include "dir_monitor.hpp"          // DirMonitor DllLoader 
#include "async_injection.hpp"      // AsyncInjection::Inject()

namespace ilrd
{

template <typename BASE, typename KEY, typename DATA>
class Framework
{
public:
    typedef Msg<KEY, DATA> *msgHandler_t;
    typedef std::tuple<int, Mode, msgHandler_t> fdModeMsg_t;

    Framework(const std::vector<fdModeMsg_t>& messages, 
              const std::string& folderPath = "./");
    ~Framework();

    Framework(const Framework&) = delete;
    Framework& operator=(const Framework&) = delete;

    static void Async(std::function<bool(void)> action,
                      std::chrono::milliseconds interval);

private:
    class ReactorFunctor
    {
    public:
        ReactorFunctor(msgHandler_t msg_cb, ThreadPool *tp);
        void operator()();

    private:
        fdModeMsg_t m_msg;
        ThreadPool *m_tp;
    };

    Reactor m_reactor;
    ThreadPool m_tp;
    DirMonitor m_dirMonitor;
    DllLoader m_dllLoader;
    std::thread m_reactorThread;

    void ThreadRoutine(const std::vector<fdModeMsg_t> &messages);
    void DirMonitorHandler(const std::string& folderPath);
};

/*---------------------------- Framework Impl --------------------------------*/

template <typename BASE, typename KEY, typename DATA>
Framework<BASE, KEY, DATA>::Framework(const std::vector<fdModeMsg_t>& messages,
                                      const std::string& folderPath) : 
                                      m_dirMonitor(folderPath)
{
    if (messages.empty())
    {
        Handleton<Logger>::GetInstance()->Log("Messages vector is empty.");
        throw std::runtime_error("Messages vector is empty");
    }
    m_reactorThread = std::thread(&Framework::ThreadRoutine, this, messages);
    DirMonitorHandler(folderPath);
    Handleton<Logger>::GetInstance()->Log("Framework v0.4 is ON.");
}

template <typename BASE, typename KEY, typename DATA>
Framework<BASE, KEY, DATA>::~Framework()
{
    m_reactor.Stop();

    if (m_reactorThread.joinable())
    {
        m_reactorThread.join();
    }
    Handleton<Logger>::GetInstance()->Log("Framework v0.4 is OFF");
}

template <typename BASE, typename KEY, typename DATA>
void Framework<BASE, KEY, DATA>::ThreadRoutine(
                                       const std::vector<fdModeMsg_t> &messages)
{
    std::for_each(messages.begin(), messages.end(), 
    [&](const fdModeMsg_t& msg_cb)
    {
       m_reactor.Add(std::get<0>(msg_cb), Reactor::Mode(std::get<1>(msg_cb)), 
                                    ReactorFunctor(std::get<2>(msg_cb), &m_tp));
    });

    m_reactor.Run();
}

template <typename BASE, typename KEY, typename DATA>
Framework<BASE, KEY, DATA>::ReactorFunctor::ReactorFunctor(msgHandler_t msg_cb, 
                                       ThreadPool *tp) : m_msg(msg_cb), m_tp(tp) 
{
}

template <typename BASE, typename KEY, typename DATA>
void Framework<BASE, KEY, DATA>::ReactorFunctor::operator()()
{
    (*m_msg)();
    auto key = m_msg->GetKey();
    auto data = m_msg->GetData();

    m_tp->AddTask(std::make_shared<std::function<int(void)>>([this, key, data]()
    {
        auto factory = Handleton<FactorySingleton<BASE, KEY, 
                                                          DATA>>::GetInstance();   
        try
        {
            auto command = factory->Create(m_msg->GetKey(), m_msg->GetData());
            command->Run(m_msg_cb->GetData());
            delete command;
            command = 0;
        }
        catch(const std::exception& e)
        {
            Handleton<Logger>::GetInstance()->Log(e.what());
            std::cerr << e.what() << endl;
        }
                                                          
        return 0;
    }), ThreadPool::MID);
}

template <typename BASE, typename KEY, typename DATA>
void Framework<BASE, KEY, DATA>::DirMonitorHandler(const std::string& 
                                                                     folderPath)
{
    m_dllLoader.Register(&m_dirMonitor);

    for (const auto& entry : std::filesystem::directory_iterator(folderPath))
    {
        try
        {
            m_dllLoader.Load(entry.path());
        }
        catch (const std::exception& e)
        {
            Handleton<Logger>::GetInstance()->Log(e.what());
        }
    }

    try 
    {
        m_dirMonitor.Run();
    }
    catch (const std::exception& e)
    {
        Handleton<Logger>::GetInstance()->Log(e.what());
    }
}

template<typename BASE, typename KEY, typename DATA> 
void Framework<BASE, KEY, DATA>::Async(std::function<bool(void)> action,
                                       std::chrono::milliseconds interval)
{
    AsyncInjection::Inject(action, interval);
}

}   // namespace ilrd

#endif /* ILRD_RD136_7_FRAMEWORK_V_0_4_HPP */