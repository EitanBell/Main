/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Ariel		              *
*	Date: 16/06/23	                  *
*	Subject: reactor		   		  *
*				                	  *
**************************************/	

#ifndef ILRD_RD136_7_REACTOR_HPP
#define ILRD_RD136_7_REACTOR_HPP

#include <vector>           // std::vector
#include <functional>       // std::function<>
#include <unordered_map>    // unordered_map

namespace ilrd
{

class Reactor
{
public:
    enum Mode : int
    {
        READ,
        WRITE,
        EXCEPT,
        NUM_OF_MODES
    };

    Reactor();
    ~Reactor() = default;

    Reactor(const Reactor&) = delete;
    Reactor& operator=(const Reactor&) = delete;

    void Add(int fd, Mode mode, const std::function<void(void)>& callback);
    void Remove(int fd, Mode mode);

    void Run(); // blocking
    void Stop();

    typedef std::pair<int, Mode> fd_mode_t;
private:

    class FDListener
    {
    public:

        FDListener(int timeoutSec);
        ~FDListener() = default;

        FDListener(const FDListener&) = delete;
        FDListener& operator=(const FDListener&) = delete;
        
        std::vector<fd_mode_t> Wait(const std::vector<fd_mode_t>& fdVec);
    private:
        int m_timeoutSec;
    };

    class Hash
    {
    public:
        std::size_t operator()(const fd_mode_t& fd) const
        {
            return std::hash<int>{}(fd.first);
        }
    };
    
    std::vector<fd_mode_t> m_fdVec;
    std::unordered_map<fd_mode_t, std::function<void(void)>, Hash> m_callbacks;
    bool m_isRunning;

    void InvokeFuncUntilSTOP(std::vector<fd_mode_t>& fdVec);
};

}   // namespace ilrd

#endif /* ILRD_RD136_7_REACTOR_HPP */   