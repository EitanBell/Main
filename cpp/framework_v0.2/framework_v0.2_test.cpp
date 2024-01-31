/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Date: 03/07/23                    *
*   Reviewer:   	                  *
*	Subject: Framework v0.2 		  *
**************************************/

#include <iostream>		   // cout
#include <unistd.h>        // STDIN_FILENO
#include <cstring>         // memcpy, memset
#include <sys/wait.h>      // waitpid
#include <sys/socket.h>    // socketpair

#include "framework_v0.2.hpp"      //Framework v0.2 API	
#include "fw_plugins_v0.2.hpp"     // Framework plugins

static const std::string PASS_COLOR = "\033[0m;32m\xE2\x9C\x93\033[0m ";

using namespace ilrd;

struct NBDRequest
{
    int m_magicNum;
    int m_reqType;
    char m_handle[8];
    size_t m_offset;
    size_t m_len;
    char m_buffer[1];
};

void FrameworkPluginsTest(void);

std::mutex g_mutex;
bool isJoinable = false;

/*---------------------------------- main ------------------------------------*/

int main(void)
{
    FrameworkPluginsTest();

    std::cout << "All tests passed " << PASS_COLOR << std::endl;

    return 0;
}

std::shared_ptr<NBDRequest> GetReq()
{
    static int reqNum = 0;
    const int MSG_SIZE = 512;

    void *req = operator new(offsetof(NBDRequest, m_buffer) + MSG_SIZE);
    std::shared_ptr<NBDRequest> newReq(new(req) NBDRequest);
    bzero(newReq.get(), offsetof(NBDRequest, m_buffer) + MSG_SIZE);

    newReq->m_magicNum = reqNum * 10;
    newReq->m_offset = offsetof(NBDRequest, m_buffer);
    newReq->m_reqType = reqNum & 1;

    std::string msg;
    std::cout << "Enter a key (or exit): " << std::endl;
    std::cin >> msg;
    newReq->m_len = msg.size();

    std::memcpy(newReq->m_buffer, msg.c_str(), newReq->m_len);

    ++reqNum;
    reqNum %= 5;

    return newReq;
}

void WriteMsgToBuffer(int socket[])
{
    const int WRITE_END = 1;

    while (1)
    {
        g_mutex.lock();
        std::shared_ptr<NBDRequest> req = GetReq();
        g_mutex.unlock();

        if (std::string(req->m_buffer) == "exit")
        {
            break;
        }

        write(socket[WRITE_END], req.get(), (sizeof(NBDRequest) + req->m_len)); 
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    isJoinable = true;

    close(socket[WRITE_END]);
}

/*--------------------------------- UserMsg ----------------------------------*/

typedef ilrd::Framework<ilrd::BaseCommand, std::string, std::string> 
                                                                    Framework_t;

class UserMsg : public Framework_t::Msg
{
public:
    UserMsg(int fd);
    ~UserMsg() = default;

    std::string GetKey() const;
    std::string GetData() const;
    void operator()();

private:
    int m_fd;
    std::string m_key;
    std::string m_data;
};

UserMsg::UserMsg(int fd) : m_fd(fd) {}

std::string UserMsg::GetKey() const
{
    return m_key;
}

std::string UserMsg::GetData() const
{
    return m_data;
}

void UserMsg::operator()()
{
    const int MAX_MSG = 1024;
    char buffer[MAX_MSG] = {'\0'};
    int n = read(m_fd, buffer, sizeof(buffer));
    
    if (n > 0)
    {
        NBDRequest *req = reinterpret_cast<NBDRequest *>(buffer);
        m_key = req->m_buffer;
        m_data = req->m_buffer;
    }
    else
    {
        throw std::runtime_error("read 0 bytes");
    }
}

/*--------------------------- FrameworkPluginsTest ---------------------------*/

void FrameworkPluginsTest(void)
{
    std::string folderPath = "../fw_plugins_v0.2/";
    std::cout << std::endl << "FRAMEWORK PLUGINS TEST:" << std::endl;
    
    const int READ_END = 0;
    const int WRITE_END = 1;
    std::vector<Framework_t::fd_mode_msg_t> messages;
    
    int socketFd[2] = {0};
    if (-1 == socketpair(AF_UNIX, SOCK_STREAM, 0, socketFd))
    {
        throw std::runtime_error("socketpair failed");
    }

    UserMsg msg(socketFd[READ_END]);
    messages.push_back(std::make_tuple(socketFd[READ_END],
                                                Framework_t::Mode::READ, &msg));

    std::thread writer(&WriteMsgToBuffer, socketFd);
    Framework_t fw(messages, folderPath);

    while (!isJoinable){}
    if (writer.joinable())
    {
        writer.join();
    }

    close(socketFd[WRITE_END]);
}