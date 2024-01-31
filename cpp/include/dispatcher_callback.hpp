/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Matan  	              *
*	Date: 08/06/23	                  *
*	Subject: Dispatcher Callback  	  *
**************************************/	

#ifndef ILRD_RD136_7_DISPATCHER_CALLBACK_HPP
#define ILRD_RD136_7_DISPATCHER_CALLBACK_HPP

#include <functional>       // std::function
#include <vector>           // std::vector<>
#include <algorithm>        // std::find

namespace ilrd
{

// EVENT must be copyable

template <class EVENT>
class Dispatcher;

/*--------------------------- class CallbackBase -----------------------------*/

template <class EVENT>
class CallbackBase
{
public:
    CallbackBase() = default;
    virtual ~CallbackBase() = default;

    CallbackBase(const CallbackBase&) = delete;
    CallbackBase& operator=(const CallbackBase&) = delete;
    
    virtual void Notify(EVENT event) =0;
    virtual void Close() =0;
    void Unregister();

private:
    void SetDispatcher(Dispatcher<EVENT> *dispatcher);
    friend Dispatcher<EVENT>;

    Dispatcher<EVENT> *m_dispatcher = nullptr;
};

/*------------------------------ class Callback ------------------------------*/

template <class OBS, class EVENT>
class Callback : public CallbackBase<EVENT>
{
    typedef void (OBS::*NotifyFunc_t)(EVENT event);
    typedef void (OBS::*CloseFunc_t)();

public:
    Callback(OBS *observer, NotifyFunc_t notify, CloseFunc_t close);
    ~Callback();

    void Notify(EVENT event);
    void Close();

private:
    OBS *m_observer;
    NotifyFunc_t m_notify;
    CloseFunc_t m_close;
};

/*----------------------------- class Dispatcher -----------------------------*/

template <class EVENT>
class Dispatcher
{
public:
    Dispatcher() = default;
    ~Dispatcher() = default;

    Dispatcher(const Dispatcher&) = delete;
    Dispatcher& operator=(const Dispatcher&) = delete;

    void Register(CallbackBase<EVENT> *callback);
    void Unregister(CallbackBase<EVENT> *callback);
    void NotifyAll(EVENT event);
    void CloseAll();

private:
    std::vector<CallbackBase<EVENT> *> m_callbacks;
};

/*------------------------- Implement CallbackBase ---------------------------*/

template<class EVENT> 
void CallbackBase<EVENT>::SetDispatcher(Dispatcher<EVENT> *dispatcher)
{
    m_dispatcher = dispatcher;
}

template<class EVENT> 
void CallbackBase<EVENT>::Unregister()
{
    if (m_dispatcher != nullptr)  
    {  
        m_dispatcher->Unregister(this);
        m_dispatcher = nullptr;
    }
}

/*--------------------------- Implement Callback -----------------------------*/

template<class OBS, class EVENT> 
Callback<OBS, EVENT>::Callback(OBS *observer, NotifyFunc_t notify, 
   CloseFunc_t close): m_observer(observer), m_notify(notify), m_close(close) {}

template <class OBS, class EVENT>
Callback<OBS, EVENT>::~Callback() 
{
    CallbackBase<EVENT>::Unregister();
}

template<class OBS, class EVENT> 
void Callback<OBS, EVENT>::Notify(EVENT event)
{
    (m_observer->*m_notify)(event);
}

template<class OBS, class EVENT> 
void Callback<OBS, EVENT>::Close()
{
    (m_observer->*m_close)();
}

/*--------------------------- I1mplement Dispatcher ---------------------------*/

template<class EVENT> 
void Dispatcher<EVENT>::Register(CallbackBase<EVENT> *callback)
{
    callback->SetDispatcher(this);
    m_callbacks.push_back(callback);
}

template<class EVENT>
void Dispatcher<EVENT>::Unregister(CallbackBase<EVENT> *callback)
{
    auto iter = std::find(m_callbacks.begin(), m_callbacks.end(), callback);

    if (m_callbacks.end() != iter)
    {
        m_callbacks.erase(iter);
    }
}

template<class EVENT> 
void Dispatcher<EVENT>::NotifyAll(EVENT event)
{
    auto cb_copy = m_callbacks;

    for (const auto& cb_iter : cb_copy)
    {
        cb_iter->Notify(event);
    }
}

template<class EVENT> 
void Dispatcher<EVENT>::CloseAll()
{
    for (const auto& cb_iter : m_callbacks)
    {
        cb_iter->Close();
        cb_iter->SetDispatcher(nullptr);
    }
    
    m_callbacks.clear();
}

}   // namespace ilrd

#endif /* ILRD_RD136_7_DISPATCHER_CALLBACK_HPP */