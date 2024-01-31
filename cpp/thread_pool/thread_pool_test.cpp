
#include <string>   		// string
#include <iostream>     	// ostream
#include <cstddef>  	    // size_t
#include <atomic>			// atomic
#include <mutex>			// mutex
#include <exception>		// throw
#include <unordered_set>	// unordered_set

#include "thread_pool.hpp" // Function Declarations

/*---------------------------------Namespaces---------------------------------*/

using namespace ilrd;
using std::cout;
using std::endl;

/*--------------------------------Global Scope--------------------------------*/

const char *g_default = "\033[0m";
const char *g_green = "\033[32m";
const char *g_red = "\033[31m";

std::atomic<int> g_counter(0);

typedef std::function<int(void)> testFunc_t;

/*---------------------------------Test Class---------------------------------*/

class Test
{
public:
    Test(std::string str);
    ~Test();
    Test(const Test& other) = delete;
    void operator=(const Test& other) = delete;

    void RunUnit(int unitResult);

private:
    std::string m_testName;
    int m_failedTests;
};

Test::Test(std::string str): m_testName(str), m_failedTests(0)
{
    cout << "\n\n\033[1;4m" << m_testName << " Tests:\033[m" << endl;
}

Test::~Test()
{
    if (0 != m_failedTests)
    {
        cout << "\033[0;31m\n\n\033[1mFailed in " << m_failedTests
            << " tests.\n" << endl;
    }
    else
    {
        cout << "\033[0;32m\n\n\033[1mPassed all " << m_testName
             << " tests!\n" << endl;
    }
	cout << "\033[0m";
}

void Test::RunUnit(int unitResult)
{
    m_failedTests += unitResult;
}

/*---------------------------------Unit Class---------------------------------*/

class Unit
{
public:
    Unit(std::string str);
    ~Unit();
    Unit(const Unit& other) = delete;
    void operator=(const Test& other) = delete;

    void RunSubTest(std::string testName, bool checkCondition);
    void RunSubTest(std::string testName, bool checkCondition, std::size_t testNum);
    int GetFailedTests();

    const std::string CHECK_MARK = "\t\033[0;32m\xE2\x9C\x93\033[0m\n";
    const std::string FAIL_MARK = "\t\033[0;31m\xE2\x9C\x97\033[0m\n";

private:
    std::string m_unitName;
    int m_failedSubTests;
};

Unit::Unit(std::string str): m_unitName(str), m_failedSubTests(0)
{
    cout << "\n\n\033[1m" << m_unitName << " tests:\033[m\n\n" << endl;
}

Unit::~Unit()
{
    cout << endl;
    if (m_failedSubTests)
	{
		cout << "\033[0;31mFailed in " << m_failedSubTests << " tests." << endl;
	}
	else
	{
		cout << "\033[0;32mPassed all " << m_unitName << " tests.\n\n";
	}	
	cout << "\033[0m";
}

void Unit::RunSubTest(std::string testName, bool checkCondition)
{
    cout << testName << " test:";

    int status = (!checkCondition);
    m_failedSubTests += status;
    
    if (status)
	{
		cout << FAIL_MARK;
	}
	else
	{
		cout << CHECK_MARK;
	}
}

void Unit::RunSubTest(std::string testName, bool checkCondition, std::size_t testNum)
{
    
    cout << testName << " test no. " << testNum << ":";
    
    int status = (!checkCondition);
    m_failedSubTests += status;
    
    if (status)
	{
		cout << FAIL_MARK;
	}
	else
	{
		cout << CHECK_MARK;
	}
}

int Unit::GetFailedTests()
{
    return (m_failedSubTests);
}

/*----------------------------------------------------------------------------*/
/*                                    Test                                    */
/*----------------------------------------------------------------------------*/

/*-----------------------------Units Declarations-----------------------------*/

void BlackBoxTests(void);
int BasicFunctions(void);
int StatusFunctions(void);
int PrintPriority(void);
int Pause(void);
int Exception(void);
int Shutdown(void);
int WaitTermination(void);
int SetNum(void);
int SuperTest(void);

/*-------------------------Static Function Declarations-----------------------*/

static int IncCounter(void);
static int WaitTime(void);
static int ThrowException(void);
std::size_t PrintBenchmark(ThreadPool *tp, unsigned int tasksNum);

/*-------------------------------Test Definition------------------------------*/

int main()
{
	Test test("Thread Pool");
	
	// test.RunUnit(BasicFunctions());
	// test.RunUnit(StatusFunctions());
	// test.RunUnit(PrintPriority());
	test.RunUnit(Pause());
	test.RunUnit(Exception());
	test.RunUnit(Shutdown());
	test.RunUnit(WaitTermination());
	// test.RunUnit(SetNum());
    test.RunUnit(SuperTest());

	return 0;
}

int BasicFunctions(void)
{
	Unit u("Counter");
	int i = 0;
	int size = 4;

	ThreadPool tp;

	for (i = 0; size > i; ++i)
	{
		tp.AddTask(std::make_shared<testFunc_t>(IncCounter), ThreadPool::LOW);
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	u.RunSubTest("Counter", size == g_counter);

	return (0 != u.GetFailedTests());
}

int StatusFunctions(void)
{
	Unit u("Status");

	ThreadPool tp(1);

	ThreadPool::TaskFuture fut = 
		tp.AddTask(std::make_shared<testFunc_t>(WaitTime), ThreadPool::HIGH);
	
	u.RunSubTest("Status", ThreadPool::DONE != fut.GetStatus(), 1);

	u.RunSubTest("Result", false != fut.WaitResult(std::chrono::milliseconds(1000)), 1);

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	u.RunSubTest("Status", ThreadPool::DONE == fut.GetStatus(), 2);

	u.RunSubTest("Result", false == fut.WaitResult(std::chrono::milliseconds(500)), 2);

	return (0 != u.GetFailedTests());
}

int PrintPriority(void)
{
	Unit u("Print Priority");
	std::size_t i = 0;
	std::mutex os_mut;

	ThreadPool tp(2);

	for (i = 0; 20 > i; ++i)
	{
		tp.AddTask(std::make_shared<testFunc_t>([&os_mut]()
		{
			std::lock_guard<std::mutex> lock(os_mut);
			cout << g_green << "LOW" << g_default << endl;
			return 0;
		}), ThreadPool::LOW);
	}

	for (i = 0; 20 > i; ++i)
	{
		tp.AddTask(std::make_shared<testFunc_t>([&os_mut]()
		{
			std::lock_guard<std::mutex> lock(os_mut);
			cout << "MID" << endl;
			return 0;
		}), ThreadPool::MID);
	}

	for (i = 0; 20 > i; ++i)
	{
		tp.AddTask(std::make_shared<testFunc_t>([&os_mut]()
		{
			std::lock_guard<std::mutex> lock(os_mut);
			cout << g_red << "HIGH" << g_default << endl;
			return 0;
		}), ThreadPool::HIGH);
	}

	return (0 != u.GetFailedTests());
}

int Pause(void)
{
	Unit u("Pause");
	std::size_t i = 0;

	ThreadPool tp(8);

	tp.Pause();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	std::vector<ThreadPool::TaskFuture> fut;
	
	for (i = 0; 4 > i; ++i)
	{
		fut.emplace_back(tp.AddTask(std::make_shared<testFunc_t>(WaitTime),
							ThreadPool::HIGH));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	for (i = 0; 4 > i; ++i)
	{
		u.RunSubTest("Status Before Resume", ThreadPool::WAIT == fut[i].GetStatus(), i + 1);
	}
	cout << endl;

	tp.Resume();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	for (i = 0; 4 > i; ++i)
	{
		u.RunSubTest("Status Before Resume", ThreadPool::WAIT != fut[i].GetStatus(), i + 1);
	}
	cout << endl;

	return (0 != u.GetFailedTests());
}

int Exception(void)
{
	Unit u("Exception");
	std::size_t i = 0;

	ThreadPool tp;

	for (i = 0; 5 > i; ++i)
	{
		tp.AddTask(std::make_shared<testFunc_t>(ThrowException), ThreadPool::MID);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	cout << "If you see this message,";
	return (0 != u.GetFailedTests());
}

int Shutdown(void)
{
	Unit u("Shutdown");

	std::size_t i = 0;

	ThreadPool tp(4);

	std::vector<ThreadPool::TaskFuture> fut;
	
	for (i = 0; 10 > i; ++i)
	{
		fut.emplace_back(tp.AddTask(std::make_shared<testFunc_t>(WaitTime),
							ThreadPool::HIGH));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	tp.Shutdown();
	cout << "Shutdown Returned" << endl << endl;

	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	for (i = 0; 10 > i; ++i)
	{
		u.RunSubTest("Status After Shutdown", ThreadPool::DONE == fut[i].GetStatus(), i + 1);
	}
	cout << endl;

	return (0 != u.GetFailedTests());
}

int WaitTermination(void)
{
	Unit u("Wait Termination");

	std::size_t i = 0;

	ThreadPool tp(4);

	std::vector<ThreadPool::TaskFuture> fut;
	
	for (i = 0; 10 > i; ++i)
	{
		fut.emplace_back(tp.AddTask(std::make_shared<testFunc_t>(WaitTime),
							ThreadPool::HIGH));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	tp.Shutdown();
	cout << "Shutdown Returned" << endl << endl;
	tp.AwaitTermination();

	for (i = 0; 10 > i; ++i)
	{
		u.RunSubTest("Status After Shutdown", ThreadPool::DONE == fut[i].GetStatus(), i + 1);
	}
	cout << endl;

	return (0 != u.GetFailedTests());
}

int SetNum(void)
{
	Unit u("Set Number of Threads");

	ThreadPool tp(1);
	std::size_t size = 0;

	size = PrintBenchmark(&tp, 4);
	u.RunSubTest("Number of Threads", 4 == size);
	cout << "uSet.size(): " << size << endl;

	size = PrintBenchmark(&tp, 40);
	u.RunSubTest("Number of Threads", 40 == size);
	cout << "uSet.size(): " << size << endl;

	size = PrintBenchmark(&tp, 10);
	u.RunSubTest("Number of Threads", 10 == size);
	cout << "uSet.size(): " << size << endl;

	size = PrintBenchmark(&tp, 200);
	u.RunSubTest("Number of Threads", 200 == size);
	cout << "uSet.size(): " << size << endl;

	size = PrintBenchmark(&tp, 2);
	u.RunSubTest("Number of Threads", 2 == size);
	cout << "uSet.size(): " << size << endl;

	return (0 != u.GetFailedTests());
}

int SuperTest(void)
{
    Unit u("Super Test");

    ThreadPool tp(10);
    std::size_t i = 0, j = 0;
    std::size_t size[3] = {50, 2000, 100000};
    std::mutex m_mtx;
    std::atomic<std::size_t> counter(0);

    for (j = 0; 3 > j; ++j)
    {
        std::vector<ThreadPool::TaskFuture> fut;
        counter.store(0);
        
        for (i = 0; size[j] > i; ++i)
        {
            fut.emplace_back(tp.AddTask(std::make_shared<testFunc_t>([&]()
            {
                std::lock_guard<std::mutex> lock(m_mtx);
                counter.fetch_add(1);
                return 0;
            }), ThreadPool::LOW));
        }

        tp.Pause();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        tp.Resume();

        for (i = 0; size[j] > i; ++i)
        {
            fut[i].WaitResult();
        }

        u.RunSubTest("Counter", size[j] == counter, j + 1);
    }

    tp.Shutdown();
	tp.AwaitTermination();

    return (0 != u.GetFailedTests());
}

/*-------------------------Static Function Definitions------------------------*/

static int IncCounter(void)
{
	g_counter.fetch_add(1);
	return 0;
}

static int WaitTime(void)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	return 0;
}

static int ThrowException(void)
{
	throw std::out_of_range ("You shouldn't let me destroy your thread pool!");

	return 0;
}

std::size_t PrintBenchmark(ThreadPool *tp, unsigned int tasksNum)
{
	std::size_t i = 0;
	std::mutex mut;
	std::unordered_set<std::thread::id> uSet;

	std::vector<ThreadPool::TaskFuture> fut;

	for (i = 0; tasksNum > i; ++i)
	{
		fut.emplace_back(tp->AddTask(std::make_shared<testFunc_t>([&]()
		{
			std::lock_guard<std::mutex> lock(mut);
			uSet.insert(std::this_thread::get_id());
			return 0;
		}), ThreadPool::MID));
	}

	for (i = 0; tasksNum > i; ++i)
	{
		fut[i].WaitResult();
	}

	return uSet.size();
}
