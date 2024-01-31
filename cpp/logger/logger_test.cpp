
#include <iostream>			// cout
#include <mutex>			//mutex
#include <future>			//std::future, async


#define I_AM_THE_HANDLETON_IMPLEMENTER
#include "handleton.hpp"    // Handleton
#include "logger.hpp"       // Class

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define TEST_RES(status) \
			(status ? ANSI_COLOR_RED "\xe2\x9c\x97 FAIL" ANSI_COLOR_RESET: \
			ANSI_COLOR_GREEN "\xE2\x9C\x93 SUCCESS" ANSI_COLOR_RESET)

using namespace ilrd;
using std::cout;
using std::endl;

static void LoggerTest(void);

int main(void)
{
    LoggerTest();
    return 0;
}


static void LoggerTest(void)
{
	auto logger = Handleton<Logger>::GetInstance();
	std::vector<std::future<void>> futures;
	std::mutex mtx;

	 for (std::size_t i = 0; 100 > i; ++i)
    {
        futures.push_back(std::async(std::launch::async, [logger, &mtx, i]
        {
            std::lock_guard<std::mutex> lock(mtx);

			logger->Log("Thread No." + std::to_string(i) + " Logged\n");
        }));
    }

	for (auto &f : futures)
    {
        f.wait();
    }
}
