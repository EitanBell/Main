	/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Ariel		              *
*	Date: 16/06/23	                  *
*	Subject: reactor		   		  *
*				                	  *
**************************************/

/*----------------------------------Includes----------------------------------*/

#include <string>       // string
#include <iostream>     // ostream
#include <cstddef>      // size_t
/*Ping Pong Test*/
#include <netdb.h>      /* struct addrinfo  */
#include <string.h>     /* memset           */
#include <unistd.h>     /* close            */
#include <arpa/inet.h>  /* inet_ntop        */

#include "reactor.hpp"   // Class

/*---------------------------------Namespaces---------------------------------*/

using namespace ilrd;
using std::cout;
using std::cin;
using std::endl;

/*--------------------------------Global Scope--------------------------------*/

const char *g_default = "\033[0m";
const char *g_green = "\033[32m";
const char *g_red = "\033[31m";

const std::size_t MAX_LEN = 80;

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

int BasicTest(void);
int PingPongTest(void);

/*-------------------------Static Function Declarations-----------------------*/

static void UDPBasicInitHints(struct addrinfo *hints)
{
    memset(hints, 0, sizeof(struct addrinfo));
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_DGRAM;
}

static struct addrinfo *UDPBindSocketToPort(struct addrinfo *run,
                                  struct addrinfo *server_info, int *socket_fd)
{
    int is_bound = false;
    struct addrinfo *found = NULL;

    for (run = server_info; (!is_bound) && (NULL != run); run = run->ai_next)
    {
        if (-1 == (*socket_fd = socket(run->ai_family, run->ai_socktype,
                                      run->ai_protocol)))
        {
            perror("listener: socket");
        }
        else if (-1 == (bind(*socket_fd, run->ai_addr, run->ai_addrlen)))
        {
            close(*socket_fd);
            perror("listener: bind");
        }
        else
        {
            is_bound = true;
            found = run;
        }
    }

    return found;
}

static void *get_in_addr(struct sockaddr *sa)
{
    if (AF_INET == sa->sa_family)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

/*-------------------------------Test Definition------------------------------*/

int main()
{
    Test test("Reactor");

    test.RunUnit(BasicTest());
    test.RunUnit(PingPongTest());
    
    return 0;
}

/*------------------------------Units Definitions-----------------------------*/

int BasicTest(void)
{
    Unit u("Basic");

    Reactor reactor;

    cout << "Interface:" << endl;
    cout << "----------" << endl;
    cout << "   'S'-> Stop" << endl;
    cout << "   'L'-> Launch" << endl;
    cout << "   'B'-> Bank Details" << endl << endl;

    reactor.Add(0, Reactor::READ, [&reactor]()
    {
        std::string buff;
        cin >> buff;

        switch (buff[0])
        {
        case 'S':
            reactor.Stop();
            break;
        case 'L':
            cout << "Launching missile..." << endl;
            break;
        case 'B':
            cout << g_red << "Shame on you!" << g_default << endl;
            break;
        default:
            cout << "Invalid Command" << endl;
            break;
        }

    });

    // reactor.Add(0, Reactor::WRITE, [&reactor]()
	// {
	// 	std::cout << "Writing" << std::endl;
	// });

    reactor.Run();

    return (0 != u.GetFailedTests());
}

int PingPongTest(void)
{
    Unit u("Ping Pong");

    int socket_fd = 0;
    struct addrinfo hints;
    struct addrinfo *server_info = NULL;
    struct addrinfo *run = NULL;
    int ret_val = 0;
    int num_bytes = 0;
    struct sockaddr_storage client_addr;
    char buf[MAX_LEN];
    socklen_t addr_len = 0;
    char s[INET6_ADDRSTRLEN];

    printf("System Hostname:\n");
    system("hostname -I");
    printf("\n");

    cout << "Please enter port num (default: 4950): "; 
    std::string port;
    cin >> port;

    cout << endl << g_red << "Enter 'Stop' to stop" << g_default << endl << endl;

    UDPBasicInitHints(&hints);
    hints.ai_flags = AI_PASSIVE;

    if (0 != (ret_val = getaddrinfo(NULL, port.c_str(), &hints, &server_info)))
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret_val));
        return 1;
    }

    run = UDPBindSocketToPort(run, server_info, &socket_fd);
    if (NULL == run)
    {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(server_info);

    printf("listener: waiting to recvfrom...\n");

    Reactor udpServer;
    udpServer.Add(socket_fd, Reactor::READ, [&]()
    {
        addr_len = sizeof(struct sockaddr_storage);
        if (-1 == (num_bytes = recvfrom(socket_fd, buf, MAX_LEN - 1, 0,
                                    (struct sockaddr *)&client_addr, &addr_len)))
        {
            perror("recvfrom");
            exit(1);
        }
        
        printf("listener: got packet from %s\n", inet_ntop(client_addr.ss_family,
                get_in_addr((struct sockaddr *)&client_addr), s, INET6_ADDRSTRLEN));
        
        printf("listener: packet is: %d bytes long\n", num_bytes);
        buf[num_bytes - 1] = '\0';
        printf("listener: packet contains: \"%s\"\n\n", buf);
    });

    udpServer.Add(0, Reactor::READ, [&udpServer]()
    {
        std::string input;
        cin >> input;

        if ("Stop" == input)
        {
            udpServer.Stop(); 
        }
    });

    udpServer.Run();

    return (0 != u.GetFailedTests());
}
