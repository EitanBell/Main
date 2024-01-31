/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Gali	        	      *
*	Date: 08/06/23	                  *
*	Subject: Logger			   		  *
*				                	  *
**************************************/
#ifndef ILRD_RD136_7_LOGGER_HPP
#define ILRD_RD136_7_LOGGER_HPP

#include <string>   //std::string
#include <queue>   //std::vector
#include <thread>   //std::thread

#include "handleton.hpp"
#include "waitable_queue.hpp"

namespace ilrd
{

class Logger
{
public:
    void Log(const std::string& stringToLog);

    Logger(const Logger& other) = delete;
    Logger& operator=(const Logger& other) = delete;

private:
    Logger();
    ~Logger();
    
    std::string TimeStamp();
    static void LoggerThread(Logger *log);
    void WriteToFile(const std::string& msgToLog);

    bool m_isRunning;
    std::thread m_thread;
    WaitableQueue<std::queue<std::string>> m_queue;
    static constexpr const char* PATH = "./";
    static constexpr const char* FILE_NAME = "logger.txt";

    friend Handleton<Logger>;
};

} //ilrd

#endif //ILRD_RD136_7_LOGGER_HPP