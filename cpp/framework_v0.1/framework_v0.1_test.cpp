/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Date: 28/06/23                    *
*   Reviewer: Noy  	                  *
*	Subject: Framework v0.1 		  *
**************************************/

#include <iostream> // cout
#include <unistd.h> // STDIN_FILENO
#include <cstring>  // memcpy, memset

#include "framework_v0.1.hpp" // fw

struct NBDRequest
{
    int m_magicNum;
    int m_reqType;
    char m_handle[8];
    std::size_t m_offset;
    std::size_t m_len;
    char m_buffer[1];
};

void FrameworkTest();
void FrameworkNBDTest(void);
NBDRequest *GetReq(void);
void WriteThread(int pipefd[]);
void FrameworkThreadPoolTest(void);

const std::string GREEN_TICK = "\033[0;32m\xE2\x9C\x93\033[0m ";

int main()
{
    FrameworkTest();
    FrameworkNBDTest();
    FrameworkThreadPoolTest();

    return 0;
}

void FrameworkTest()
{
    std::vector<std::tuple<int, ilrd::Framework::Mode, 
                    ilrd::Framework::callback_t>> callbacks;

    callbacks.push_back(std::make_tuple<int, ilrd::Framework::Mode, 
                                        ilrd::Framework::callback_t>(
        STDIN_FILENO, ilrd::Framework::Mode::READ, 
        [&]()
        {
            char buffer[100];
            read(STDIN_FILENO, buffer, 100);
            std::cout << buffer << "callback called" << std::endl; 
        }));

    ilrd::Framework fw(callbacks);

    std::cout << "Framework created" << std::endl;
    std::cout << "go to sleep" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

std::mutex g_mtx;

void FrameworkNBDTest(void)
{
    std::cout  << std::endl << "FRAMEWORK NBD TEST:" << std::endl;
    std::cout << "Write 5 times on 1 pipe with 1 thread:" << std::endl;

    const int READ_END = 0;
    std::vector<ilrd::Framework::fdModeCb_t> callbacks;

    int pipefd[2] = {0};

    if (-1 == pipe(pipefd))
    {
        throw std::runtime_error("pipe error");
    }

    callbacks.push_back(std::make_tuple((pipefd[READ_END]),
                                        ilrd::Framework::Mode::READ,
    [&]()
    {
        const int MAX_MSG = 1024;
        char buffer[MAX_MSG] = {0};

        if (read(pipefd[READ_END], buffer, sizeof(buffer)))
        {
            std::cout << "NBD Request:" << std::endl;

            NBDRequest *req = reinterpret_cast<NBDRequest *>(buffer);
            std::cout << "m_magicNum = " << req->m_magicNum << std::endl;
            std::cout << "m_len = " << req->m_len << std::endl;
            std::cout << "m_offset = " << req->m_offset << std::endl;
            std::cout << "m_reqType = " << req->m_reqType << std::endl;
            std::cout << "m_buffer = " << req->m_buffer << std::endl << std::endl;
        }
    }));

    ilrd::Framework fw(callbacks);

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::thread writeThread(&WriteThread, pipefd);
    writeThread.join();
}

void WriteThread(int pipefd[])
{
    const int WRITE_END = 1;

    for (size_t i = 0; i < 5; ++i)
    {
        g_mtx.lock();
        NBDRequest *req = GetReq();
        g_mtx.unlock();

        write(pipefd[WRITE_END], req, (sizeof(NBDRequest) + req->m_len));
        std::this_thread::sleep_for(std::chrono::seconds(1));
        delete req;
    }

    close(pipefd[WRITE_END]);
}

NBDRequest *GetReq(void)
{
    static int reqNum = 0;
    const char *message[] = {"msg 1", "msg 2", "msg 3", "msg 4", "msg 5"};
    const int MSG_SIZE = 512;

    void *req = operator new(offsetof(NBDRequest, m_buffer) + MSG_SIZE);
    NBDRequest *newReq = new(req) NBDRequest;
    memset(newReq, 0, offsetof(NBDRequest, m_buffer) + MSG_SIZE);

    newReq->m_magicNum = reqNum * 10;
    newReq->m_len = strlen(message[reqNum]);
    newReq->m_offset = offsetof(NBDRequest, m_buffer);
    newReq->m_reqType = reqNum & 1;
    std::memcpy(newReq->m_buffer, message[reqNum], newReq->m_len);

    ++reqNum;
    reqNum %= 5;

    return newReq;
}

void FrameworkThreadPoolTest(void)
{
    std::cout << "Write 5 times on 100 pipes with 100 threads:" << std::endl;

    const int READ_END = 0;
    const int NUM_OF_PIPES = 100;
    std::vector<ilrd::Framework::fdModeCb_t> callbacks;

    int pipefd[NUM_OF_PIPES][2] = {0};

    for (size_t i = 0; i < NUM_OF_PIPES; ++i)
    {
        if (-1 == pipe(pipefd[i]))
        {
            throw std::runtime_error("pipe error");
        }
    }

    std::mutex mtx;
    std::unordered_map<std::thread::id, int> threadsTasks;
    int readCntr = 0;

    for (size_t i = 0; i < NUM_OF_PIPES; ++i)
    {
        callbacks.push_back(std::make_tuple((pipefd[i][READ_END]),
                                            ilrd::Framework::Mode::READ,
        [&, i]()
        {
            const int MAX_MSG = 1024;
            char buffer[MAX_MSG] = {0};

            if (0 < read(pipefd[i][READ_END], buffer, sizeof(buffer)))
            {
                std::lock_guard<std::mutex> lock(mtx);
                ++(threadsTasks[std::this_thread::get_id()]);
                ++readCntr;
            }
        }));
    }

    {
        ilrd::Framework fw(callbacks);

        std::thread writeThreads[NUM_OF_PIPES];

        for (size_t i = 0; i < NUM_OF_PIPES; ++i)
        {
            writeThreads[i] = std::thread(&WriteThread, pipefd[i]);
        }

        for (size_t i = 0; i < NUM_OF_PIPES; ++i)
        {
            writeThreads[i].join();
        }
    }

    std::cout << "Num of threads = " << threadsTasks.size() << std::endl;
    std::cout << "std::thread::hardware_concurrency() = " << 
                std::thread::hardware_concurrency() << std::endl << std::endl;

    std::cout << "Check Threads: " << std::endl << std::endl;

    int tasksCntr = 0;
    for (auto it = threadsTasks.begin(); it != threadsTasks.end(); ++it)
    {
        std::cout << "thread id: " << it->first << " -> ran " << it->second << " tasks" << std::endl;
        tasksCntr += it->second;
    }

    std::cout << "> Total tasks: " << tasksCntr << std::endl;
    std::cout << "> Read Counter: " << readCntr << std::endl;

    std::cout << "> Num of threads test: ";
    (threadsTasks.size() == std::thread::hardware_concurrency()) ? 
        (std::cout << "Succeed " << GREEN_TICK << std::endl) : (std::cout << "Fail" << std::endl);

    std::cout << "> Num of tasks test: ";
    (tasksCntr == readCntr) ?
        (std::cout << "Succeed " << GREEN_TICK << std::endl) : (std::cout << "Fail" << std::endl);
}
