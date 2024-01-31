/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Gali	        	      *
*	Date: 08/06/23	                  *
*	Subject: Logger			   		  *
*				                	  *
**************************************/

#include <ctime>        // time
#include <iomanip>      // put_time
#include <fstream>      // ofstream
#include <sstream>      // stringstream

#include "logger.hpp"	// logger API 

namespace ilrd 
{

/*----------------------------------- Logger ---------------------------------*/

Logger::Logger(): m_isRunning(true), m_thread(std::thread(&LoggerThread, 
																		this)){}

Logger::~Logger()
{
    m_isRunning = false;
    Log("--- Logger closed ---");
    m_thread.join();
}

void Logger::Log(const std::string& stringToLog)
{
    std::string msgWithTime = TimeStamp() + stringToLog;
    
    m_queue.Push(msgWithTime);
}

void Logger::LoggerThread(Logger *log)
{
    while (log->m_isRunning || !(log->m_queue.IsEmpty()))
    {
        std::string msgToLog;
        log->m_queue.Pop(msgToLog);

        log->WriteToFile(msgToLog);
    }
}

std::string Logger::TimeStamp()
{
	std::time_t time = std::time(nullptr);
    std::tm tm = *std::localtime(&time);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d/%m/%Y %X\t");
    
	return oss.str();
}

void Logger::WriteToFile(const std::string& msgToLog)
{
	std::ofstream file;
	file.open(std::string(PATH) + FILE_NAME, std::ios::app);
	file << msgToLog << std::endl;
	file.close();
}

} // ilrd namespace


