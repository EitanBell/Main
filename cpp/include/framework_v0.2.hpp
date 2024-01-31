/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Date: 03/07/23                    *
*   Reviewer: Eliran                  *
*	Subject: Framework v0.2 		  *
**************************************/

#ifndef ILRD_RD136_7_FRAMEWORK_V_0_2_HPP
#define ILRD_RD136_7_FRAMEWORK_V_0_2_HPP

/*
THIS VERSION (V0.3) INCLUDES REACTOR, THREADPOOL, LOGGER, FACTORY AND PLUGINS. 
*/

#include <functional>      // std::function<>
#include <thread>          // threads
#include <vector>          // vector
#include <tuple>           // tuple
#include <filesystem>      // filesystem

#include "reactor.hpp"              // Reactor API
#include "thread_pool.hpp"          // ThreadPool API
#include "logger.hpp"               // Logger API
#include "factory_singleton.hpp"    // Factory Singleton API
#include "dir_monitor.hpp"          // DirMonitor DllLoader API

namespace ilrd
{

template <typename BASE, typename KEY, typename DATA>
class Framework
{
public:
    enum Mode : int
    {
        READ,
        WRITE,
        EXCEPT
    };

    class Msg
    {
    public:
        virtual ~Msg() = default;
        virtual KEY GetKey() const =0;
        virtual DATA GetData() const =0;
        virtual void operator()() =0;
    };

    typedef std::tuple<int, Mode, Msg*> fd_mode_msg_t;

    Framework(const std::vector<fd_mode_msg_t>& messages, 
              const std::string& pluginsDir = "./");
    ~Framework();

    Framework(const Framework&) = delete;
    Framework& operator=(const Framework&) = delete;

private:
    class ReactorFunctor
    {
    public:
        ReactorFunctor(Msg *msg_cb, ThreadPool *tp);
        void operator()() const;

    private:
        Msg *m_msg_cb;
        ThreadPool *m_tp;
    };

    Reactor m_reactor;
    ThreadPool m_tp;
    DirMonitor m_dirMonitor;
    DllLoader m_dllLoader;
    std::thread m_thread;

    std::thread InitThread(const std::vector<fd_mode_msg_t>& messages);
    void LoadPlugins(const std::string& pluginsDir);
};

/*---------------------------- Framework Impl --------------------------------*/

template <typename BASE, typename KEY, typename DATA>
Framework<BASE, KEY, DATA>::Framework(
                                   const std::vector<fd_mode_msg_t>& messages,
                                   const std::string& pluginsDir) : 
                                   m_dirMonitor(pluginsDir),
                                   m_thread(InitThread(messages))
{
    LoadPlugins(pluginsDir);
    Handleton<Logger>::GetInstance()->Log("Framework v0.2 is ON.");
}

template <typename BASE, typename KEY, typename DATA>
Framework<BASE, KEY, DATA>::~Framework()
{
    m_reactor.Stop();

    if (m_thread.joinable())
    {
        m_thread.join();
    }

    Handleton<Logger>::GetInstance()->Log("Framework v0.2 is OFF.");
}

template <typename BASE, typename KEY, typename DATA>
std::thread Framework<BASE, KEY, DATA>::InitThread(
                                     const std::vector<fd_mode_msg_t>& messages)
{
    if (0 == messages.size())
    {
        Handleton<Logger>::GetInstance()->Log("messages vector is empty.");
        throw std::invalid_argument("messages vector is empty.");
    }

    for (std::size_t i = 0; i < messages.size(); ++i)
    {
        m_reactor.Add(std::get<0>(messages[i]), 
                    static_cast<Reactor::Mode>(std::get<1>(messages[i])), 
                    ReactorFunctor(std::get<2>(messages[i]), &m_tp));
    }

    return std::thread(&Reactor::Run, &m_reactor);
}

template <typename BASE, typename KEY, typename DATA>
Framework<BASE, KEY, DATA>::ReactorFunctor::ReactorFunctor(Msg *msg_cb, 
                                    ThreadPool *tp) : m_msg_cb(msg_cb), m_tp(tp) 
{
}

template <typename BASE, typename KEY, typename DATA>
void Framework<BASE, KEY, DATA>::ReactorFunctor::operator()() const
{
    (*m_msg_cb)();
    auto key = m_msg_cb->GetKey();
    auto data = m_msg_cb->GetData();

    m_tp->AddTask(std::make_shared<std::function<int(void)>>([this]()
    {
        auto factory = Handleton<FactorySingleton<BASE, KEY,    
                                                          DATA>>::GetInstance();
        BASE *command = factory->Create(m_msg_cb->GetKey(), 
                                                           m_msg_cb->GetData());
        command->Run(m_msg_cb->GetData());
        delete command;
        command = 0;

        return 0;
    }), ThreadPool::MID);
}

template <typename BASE, typename KEY, typename DATA>
void Framework<BASE, KEY, DATA>::LoadPlugins(const std::string& pluginsDir)
{
    m_dllLoader.Register(&m_dirMonitor);

    for (const auto& entry : std::filesystem::directory_iterator(pluginsDir))
    {
        m_dllLoader.Load(entry.path());
    }

    m_dirMonitor.Run();
}

}   // namespace ilrd

#endif /* ILRD_RD136_7_FRAMEWORK_V_0_2_HPP */