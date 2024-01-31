/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Shauli  	              *
*	Date: 12/06/23	                  *
*	Subject: DLL Loader, Dir Monitor  *
**************************************/	

#ifndef ILRD_RD136_7_DIR_MONITOR_HPP
#define ILRD_RD136_7_DIR_MONITOR_HPP

#include <string>           // std::string
#include <thread>           // std::thread
#include <sys/inotify.h>    // inotify
#include <unordered_map>    // std::unordered_map
#include <sys/select.h>     // fd_set
#include <limits.h>         // NAME_MAX

#include "dispatcher_callback.hpp"

namespace ilrd
{

/****************************** Class DirMonitor ******************************/

class DirMonitor
{
public:
    DirMonitor(std::string folderPath);
    ~DirMonitor();

    void Register(CallbackBase<std::string> *callback);
    void Unregister(CallbackBase<std::string> *callback);
    void Run();
    void Stop();

private:
    void NotifyAll(std::string pluginName);
    void CloseAll();

    Dispatcher<std::string> m_disp;
    std::string m_folderPath;
    std::thread m_thread;
    bool m_isRunning;
    int m_inotifyFD;
    int m_watchFD;

    int InitInotify();
    int InitWatch(const std::string& folderPath);
    static void ThreadRoutine(DirMonitor *monitor);
    void SetTimeout(struct timeval& timeout, std::size_t seconds);
    void ReadDir();
    void SetReadFDMonitor(DirMonitor *monitor, fd_set& read_set);

    typedef struct inotify_event inotify_t;

    static const std::size_t MAX_EVENTS = 1024;
    static const std::size_t LEN_NAME = NAME_MAX + 1;
    static const std::size_t EVENT_SIZE = sizeof(inotify_t);
    static const std::size_t BUF_LEN = MAX_EVENTS * (EVENT_SIZE + LEN_NAME);
};

/*************************** Class class DllLoader ****************************/

class DllLoader
{
public:
    DllLoader();
    ~DllLoader();

    void Register(DirMonitor *dir);
    void Load(std::string pluginName); // Accept .so files only
    void Close();

private:
    Callback<DllLoader, std::string> m_callback;
    std::unordered_map<std::string, void *> m_plugins;    

    static bool IsSOFile(const std::string& pluginName);
};

}   // namespace ilrd

#endif /* ILRD_RD136_7_DIR_MONITOR_HPP */