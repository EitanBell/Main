/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Shauli  	              *
*	Date: 12/06/23	                  *
*	Subject: DLL Loader, Dir Monitor  *
**************************************/	

#include <dlfcn.h>      // dlopen, dlclose
#include <unistd.h>     // close
#include <stdexcept>    // runtime_error
#include <sys/time.h>   // struct timeval
#include <sys/types.h>
#include <sys/select.h>

#include "dir_monitor.hpp"	// DLL Loader & Dir Monitor API 

namespace ilrd
{

enum status : int
{
    FAIL = -1
};
	
/***************************** Implement DirMonitor ***************************/

DirMonitor::DirMonitor(std::string folderPath): m_disp(), 
                       m_folderPath(folderPath), m_thread(), m_isRunning(false), 
                           m_inotifyFD(InitInotify()), m_watchFD(InitWatch()) {}

DirMonitor::~DirMonitor()
{
	if (m_isRunning)
	{
		Stop();
	}

	inotify_rm_watch(m_inotifyFD, m_watchFD);
	close(m_inotifyFD);

    CloseAll();
}

void DirMonitor::Register(CallbackBase<std::string> *callback)
{
    m_disp.Register(callback);
}

void DirMonitor::Unregister(CallbackBase<std::string> *callback)
{   
    m_disp.Unregister(callback);
}

void DirMonitor::Run()
{
    if (m_isRunning)
    {
        throw std::runtime_error("Run(): DirMonitor is already running");
    }

    m_isRunning = true;

    if (m_thread.joinable())
    {
        m_thread.join();
    }

    m_thread = std::move(std::thread(&DirMonitor::ThreadRoutine, this));
}

void DirMonitor::Stop()
{
    if (!m_isRunning)
    {
        throw std::runtime_error("Stop(): DirMonitor is not running");
    }

    m_isRunning = false;

    if (m_thread.joinable())                                                  
    {
        m_thread.join();
    }
}

void DirMonitor::NotifyAll(std::string pluginName)
{
    m_disp.NotifyAll(pluginName);
}

void DirMonitor::CloseAll()
{
    m_disp.CloseAll();
}

/****************************** Implement DllLoader ***************************/

DllLoader::DllLoader(): m_callback(this, &DllLoader::Load, &DllLoader::Close){}

DllLoader::~DllLoader() 
{
    m_callback.Unregister();
    Close();
}

void DllLoader::Register(DirMonitor *dir)
{
    dir->Register(&m_callback);
}

void DllLoader::Load(std::string pluginName)
{  
    if (IsSOFile(pluginName))
    {
        auto iter = m_plugins.find(pluginName);
        if (m_plugins.end() != iter)
        {
            dlclose(m_plugins[pluginName]);
        }
        
        void *handle = dlopen(pluginName.c_str(), RTLD_LAZY);
        if (nullptr == handle)
        {
            throw std::runtime_error("Load(): Loading plugin failed.");
        }

        m_plugins[pluginName] = handle;
    }
}

void DllLoader::Close()
{
    for (auto iter : m_plugins)
    {
        if (dlclose(iter.second))
        {
            throw std::runtime_error(dlerror());
        }
    }
    
    m_plugins.clear();
}

/******************************* Helper Functions *****************************/

int DirMonitor::InitInotify()
{
    int inotifyFD = inotify_init();
    if (FAIL == inotifyFD)
    {
    	throw std::runtime_error("Ctor(): Failed to get inotify FD");
    }

    return inotifyFD;
}

int DirMonitor::InitWatch()
{
    int watchFD = inotify_add_watch(m_inotifyFD, m_folderPath.c_str(), 
                                                                IN_CLOSE_WRITE);
                                                                                   
    if (FAIL == watchFD)
    {
	    close(m_inotifyFD);
        throw std::runtime_error("Ctor(): Failed to get watch fd\n");
    }

    return watchFD;
}

void DirMonitor::ThreadRoutine(DirMonitor *monitor)
{
    fd_set read_set;
    struct timeval timeout;

    while (monitor->m_isRunning)
    {
        monitor->SetReadFDMonitor(monitor, read_set);
        monitor->SetTimeout(timeout, 10);
        
        int ready = select(monitor->m_inotifyFD + 1, &read_set, nullptr, 
                                                             nullptr, &timeout);
        if (FAIL == ready)
        {
            throw std::runtime_error("Run(): failed to read file descriptor");
        }
        else if (ready)
        {
            monitor->ReadDir();
            
        }     
    }
}

void DirMonitor::SetTimeout(struct timeval& timeout, std::size_t seconds)
{
    timeout.tv_sec = seconds;
    timeout.tv_usec = 0;
}

void DirMonitor::ReadDir()
{
    char buffer[BUF_LEN];
    int bytes_read = 0;

    int read_byte_length = read(m_inotifyFD, buffer, BUF_LEN);
    if (FAIL == read_byte_length)
    {
        throw std::runtime_error("Run(): read directory failed");
    }

    while (bytes_read < read_byte_length)
    {
        inotify_t *event = reinterpret_cast<inotify_t *>(buffer);
        NotifyAll(m_folderPath + event->name);
        bytes_read += EVENT_SIZE + event->len;
    }
}

void DirMonitor::SetReadFDMonitor(DirMonitor *monitor, fd_set& read_set)
{
    FD_ZERO(&read_set);
    FD_SET(monitor->m_inotifyFD, &read_set);
}

inline bool DllLoader::IsSOFile(const std::string& pluginName)
{
    return (pluginName.find(".so") != std::string::npos);
}

} // ilrd namespace