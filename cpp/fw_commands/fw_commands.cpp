/*********************************************
*	Developer: Eitan Bellaiche               *	 
*	Reviewer:  Eliran                        *
*	Date: 03/07/23	                         *
*	Subject: Framework plugins integration   *
*********************************************/	

#include <iostream>                 // cout
#include <mutex>                    // mutex, lock_guard
#include <atomic>                   // atomic
#include <chrono>                   // time
#include <cstring>                  // memcpy
#include <unistd.h>                 // read, write

#include "handleton.hpp"            // handleton
#include "fw_commands.hpp"          // BaseCommand, PluginGuard template


namespace ilrd
{

static int ReadAll(int fd, char *buf, size_t count)
{
  int bytes_read;

  while (count > 0)
  {
    bytes_read = read(fd, buf, count);

    buf += bytes_read;
    count -= bytes_read;
  }

  return 0;
}

static int WriteAll(int fd, char *buf, size_t count)
{
  int bytes_written;

  while (count > 0)
  {
    bytes_written = write(fd, buf, count);

    buf += bytes_written;
    count -= bytes_written;
  }

  return 0;
}

class ReadCommand : public BaseCommand
{
public:
    void Run(u_int64_t from, u_int32_t len, int socketFd, void *readThis);
};

void ReadCommand::Run(u_int64_t offset, u_int32_t len, int socketFd, 
                                                                 void *readThis)
{
    char *tmpBuffer = new char[len];

    memcpy(tmpBuffer, (char *)readThis, len);
    
    std::cout << "read from tmpBuffer " << len << " bytes" << std::endl;

    Framework<BaseCommand, std::string, std::string>::Async([]()
    {
        std::cout << "async invoked in read" << std::endl;
        return true;
    }, std::chrono::seconds(5));
}

static PluginGuard<ReadCommand> s_ReadCmd("READ");

class WriteCommand : public BaseCommand
{
public:
    void Run(u_int64_t from, u_int32_t len, int socketFd, void *writeHere);
};

void WriteCommand::Run(u_int64_t offset, u_int32_t len, int socketFd, 
                                                                void *writeHere)
{
    char *tmpBuffer = new char[len];
    
    ReadAll(socketFd, tmpBuffer, len);

    memcpy((char *)writeHere + offset, tmpBuffer, len);
    
    std::cout << "wrote to tmpBuffer " << len << " bytes" << std::endl;

    Framework<BaseCommand, std::string, std::string>::Async([]()
    {
        std::cout << "async invoked in write" << std::endl;
        return true;
    }, std::chrono::seconds(5));
}

} // namespace ilrd
