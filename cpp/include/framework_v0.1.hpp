/**************************************
*	Developer: Eitan Bellaiche         *	 
*	Date: 28/06/23                     *
*  Reviewer: Noy  	                 *
*	Subject: Framework ver_1 		     *
**************************************/

#ifndef ILRD_RD136_7_FRAMEWORK_HPP
#define ILRD_RD136_7_FRAMEWORK_HPP

#include <functional>      // std::function<>
#include <thread>          // threads
#include <vector>          // vector
#include <tuple>           // tuple

#include "reactor.hpp"     // reactor API
#include "thread_pool.hpp" // ThreadPool API

namespace ilrd
{

class Framework
{
public:
   enum Mode : int
   {
      READ,
      WRITE,
      EXCEPT
   };
   typedef std::function<void(void)> callback_t;
   typedef std::tuple<int, Mode, callback_t> fdModeCb_t;

   Framework(const std::vector<fdModeCb_t>& callbacks);
   ~Framework();

   Framework(const Framework&) = delete;
   Framework& operator=(const Framework&) = delete;

private:
   ThreadPool m_tp;
   Reactor m_reactor;
   std::thread m_thread;

   class ReactorFunctor
    {
    public:
        ReactorFunctor(callback_t cb, ThreadPool *tp);
        void operator()() const;

    private:
        callback_t m_callback;
        ThreadPool *m_tp;
    };


   std::thread InitThread(const std::vector<fdModeCb_t>& callbacks);
};

}   // namespace ilrd


#endif /* ILRD_RD136_7_FRAMEWORK_HPP */