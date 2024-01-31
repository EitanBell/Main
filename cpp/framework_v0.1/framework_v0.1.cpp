/**************************************
*   Developer: Eitan Bellaiche        *	 
*	Date: 28/06/23                    *
*   Reviewer: Noy  	                  *
*	Subject: Framework v0.1 		  *
**************************************/	
#include <algorithm>        // for_each

#include "framework_v0.1.hpp"    // framework API
#define I_AM_THE_HANDLETON_IMPLEMENTER
#include "logger.hpp"            // Logger API

namespace ilrd
{
Framework::Framework(const std::vector<fdModeCb_t>& callbacks)
                                               : m_thread(InitThread(callbacks))
{
    Handleton<Logger>::GetInstance()->Log("Framework ver_1 is ON");
}

Framework::~Framework()
{
    m_tp.AddTask(std::make_shared<std::function<int()>>([this]()
    {
        m_reactor.Stop();
        return 0;
    }), ThreadPool::LOW);
    
    if (m_thread.joinable())
    {
        m_thread.join();
    }

    Handleton<Logger>::GetInstance()->Log("Framework ver_1 is OFF");  
}

Framework::ReactorFunctor::ReactorFunctor(callback_t cb, ThreadPool *tp)
                                                  : m_callback(cb),  m_tp(tp) {}

void Framework::ReactorFunctor::operator()() const
{
    m_tp->AddTask(std::make_shared<std::function<int(void)>>([this]()
    {
        m_callback();
        return 0;
    }), ThreadPool::MID);
}

std::thread Framework::InitThread(const std::vector<fdModeCb_t>& callbacks)
{
    if (callbacks.empty())
    {
        throw std::runtime_error("Error: Empty callbacks vector");
    }
    
    std::for_each(callbacks.begin(), callbacks.end(), 
                  [this](fdModeCb_t callback)
    {
        m_reactor.Add(std::get<0>(callback),
                      static_cast<Reactor::Mode>(std::get<1>(callback)),
                      ReactorFunctor(std::move(std::get<2>(callback)), &m_tp));
    });

    return std::thread(&Reactor::Run, &m_reactor);
}

} // namespace ilrd