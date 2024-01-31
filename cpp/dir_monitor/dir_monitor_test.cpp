/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Shauli  	              *
*	Date: 12/06/23	                  *
*	Subject: DLL Loader, Dir Monitor  *
**************************************/	

#include <iostream>				// cout
#include <dlfcn.h>				//dlopen
#include <vector>				//std::vector

#include "factory_singleton.hpp"
#include "dir_monitor.hpp"  	// API
#include "plugin.hpp"
#include "handleton.hpp"
#include "plugin_main.hpp"

using namespace std;
using namespace ilrd;

void SimpleTest(void);
void FactoryTest(void);

int main(void)
{
	SimpleTest();
	FactoryTest();

	return 0;
}

void SimpleTest()
{
    DirMonitor dirM("./plugins/");
    // DirMonitor dirM("../libs/debug/");
    DllLoader dllL;

    dllL.Register(&dirM);

    cout << "Loop Interface:" << endl;
    cout << "---------------" << endl;
    cout << "  'r' -> Run()" << endl;
    cout << "  's' -> Stop()" << endl;
    cout << "  'q' -> Quit loop" << endl << endl;

    char input = 'x';
    while ('q' != input)
    {
        std::cin >> input;

        switch (input)
        {
        case 'r':
            dirM.Run();
            break;
        case 's':
            dirM.Stop();
            break;
        case 'q':
            cout << "Quit loop..." << endl;
            break;
        default:
            break;
        }
    }
    cout << endl;
}

void FactoryTest(void)
{
    auto fac = Handleton<FactorySingleton<PluginBase, 
                                                   std::string>>::GetInstance();

    DirMonitor dirM("./plugins/");
    DllLoader dllL;
    dllL.Register(&dirM);
    std::vector<PluginBase *> objVec;
    dirM.Run();

    cout << "Enter 'q' to Quit loop," << endl;
    cout << "any other key to continue" << endl << endl;
    char input = 'x';
    while ('q' != input)
    {
        try
        {
            objVec.push_back(fac->Create("PluginBase"));
        }
        catch(const std::exception& e)
        {
            cout << "PluginBase not loaded" << endl;
        }
        
        try
        {
            objVec.push_back(fac->Create("PluginDerived1"));
        }
        catch(const std::exception& e)
        {
            cout << "PluginDerived1 not loaded" << endl;
        }

        try
        {
            objVec.push_back(fac->Create("PluginDerived2"));
        }
        catch(const std::exception& e)
        {
            cout << "PluginDerived2 not loaded" << endl;
        }

        cout << endl;
        for (auto &objIt : objVec)
        {
            objIt->PluginPrint();
            delete objIt;
            objIt = nullptr;
        }
        cout << endl;

        objVec.clear();

        std::cin >> input;
        cout << endl;
    }
}





